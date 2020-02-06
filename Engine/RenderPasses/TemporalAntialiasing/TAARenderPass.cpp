#include "TAARenderPass.h"

#include "TAAMapVolume.h"

#include "RenderPasses/GBuffer.h"

TAARenderPass::TAARenderPass () :
	_taaMapVolume (nullptr)
{

}

TAARenderPass::~TAARenderPass ()
{
	delete _taaMapVolume;
}

void TAARenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post process render pass
	*/

	PostProcessRenderPass::Init (settings);

	/*
	 * Initialize last post process map volume
	*/

	_taaMapVolume = CreatePostProcessVolume ();

	InitTAAMapVolume (settings);

	/*
	 * Initialize ping-pong buffers
	*/

	auto currentTAAMapVolume = (TAAMapVolume*) _postProcessMapVolume;
	auto taaMapVolume = (TAAMapVolume*) _taaMapVolume;

	currentTAAMapVolume->SetCurrent(true);
	taaMapVolume->SetCurrent(false);
}

RenderVolumeCollection* TAARenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update temporal anti-aliasing map volume
	*/

	UpdateTAAMapVolume (settings);

	/*
	 * Keep current camera view projection matrix
	*/

	rvc->Insert ("TAAMapVolume", _taaMapVolume);

	/*
	 * Execute post process render pass
	*/

	rvc = PostProcessRenderPass::Execute (renderScene, camera, settings, rvc);

	return rvc;
}

void TAARenderPass::Clear ()
{
	/*
	 * Clear last post processing volume
	*/

	_taaMapVolume->Clear ();

	/*
	 * Clear post process render pass
	*/

	PostProcessRenderPass::Clear ();
}

bool TAARenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if temporal anti-aliasing is enabled
	*/

	return settings.taa_enabled;
}

std::string TAARenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalAntialiasing/temporalAntialiasingFragment.glsl";
}

std::string TAARenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 TAARenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* TAARenderPass::CreatePostProcessVolume () const
{
	TAAMapVolume* volume = new TAAMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> TAARenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	auto gBuffer = (GBuffer*) rvc->GetRenderVolume ("GBuffer");
	auto taaMapVolume = (TAAMapVolume*) _taaMapVolume;

	PipelineAttribute frustumJitter;
	PipelineAttribute reprojectionMatrix;

	frustumJitter.type = PipelineAttribute::AttrType::ATTR_2F;
	reprojectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	frustumJitter.name = "frustumJitter";
	reprojectionMatrix.name = "reprojectionMatrix";

	frustumJitter.value = glm::vec3 (gBuffer->GetFrustumJitter (), 0.0f);

	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	glm::mat4 reprojectionMat = screenMatrix * taaMapVolume->GetViewProjectionMatrix () * glm::inverse (viewMatrix);

	reprojectionMatrix.matrix = reprojectionMat;

	attributes.push_back (frustumJitter);
	attributes.push_back (reprojectionMatrix);

	return attributes;
}

void TAARenderPass::UpdateTAAMapVolume (const RenderSettings& settings)
{
	glm::ivec2 framebufferSize = glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);

	glm::ivec2 taaMapSize = _taaMapVolume->GetSize ();

	if (taaMapSize != framebufferSize) {

		/*
		 * Clear temporal anti-aliasing map volume
		*/

		_taaMapVolume->Clear ();

		/*
		 * Initialize temporal anti-aliasing map volume
		*/

		InitTAAMapVolume (settings);
	}
}

void TAARenderPass::InitTAAMapVolume (const RenderSettings& settings)
{
	/*
	 * Initialize temporal anti-aliasing map volume
	*/

	glm::ivec2 volumeResolution = glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);

	if (!_taaMapVolume->Init (volumeResolution)) {
		Console::LogError (std::string () + "Temporal anti-aliasing volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}
