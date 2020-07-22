#include "TemporalFilterRenderPass.h"

#include "TemporalFilterMapVolume.h"

#include "RenderPasses/GBuffer.h"

#include "Core/Console/Console.h"

TemporalFilterRenderPass::TemporalFilterRenderPass () :
	_temporalFilterMapVolume (nullptr)
{

}

TemporalFilterRenderPass::~TemporalFilterRenderPass ()
{
	delete _temporalFilterMapVolume;
}

void TemporalFilterRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post process render pass
	*/

	PostProcessRenderPass::Init (settings);

	/*
	 * Initialize last post process map volume
	*/

	_temporalFilterMapVolume = CreatePostProcessVolume ();

	InitLastPostProcessMapVolume (settings);

	/*
	 * Initialize ping-pong buffers
	*/

	auto currentTemporalFilterMapVolume = (TemporalFilterMapVolume*) _postProcessMapVolume;
	auto temporalFilterMapVolume = (TemporalFilterMapVolume*) _temporalFilterMapVolume;

	currentTemporalFilterMapVolume->SetCurrent(true);
	temporalFilterMapVolume->SetCurrent(false);
}

RenderVolumeCollection* TemporalFilterRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update temporal anti-aliasing map volume
	*/

	UpdateLastPostProcessMapVolume (settings);

	/*
	 * Keep current camera view projection matrix
	*/

	rvc->Insert ("TemporalFilterMapVolume", _temporalFilterMapVolume);

	/*
	 * Execute post process render pass
	*/

	rvc = PostProcessRenderPass::Execute (renderScene, camera, settings, rvc);

	return rvc;
}

void TemporalFilterRenderPass::Clear ()
{
	/*
	 * Clear last post processing volume
	*/

	_temporalFilterMapVolume->Clear ();

	/*
	 * Clear post process render pass
	*/

	PostProcessRenderPass::Clear ();
}

std::string TemporalFilterRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalFiltering/temporalFilteringFragment.glsl";
}

std::string TemporalFilterRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 TemporalFilterRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height));
}

PostProcessMapVolume* TemporalFilterRenderPass::CreatePostProcessVolume () const
{
	TemporalFilterMapVolume* temporalFilterMapVolume = new TemporalFilterMapVolume ();

	return temporalFilterMapVolume;
}

std::vector<PipelineAttribute> TemporalFilterRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	auto gBuffer = (GBuffer*) rvc->GetRenderVolume ("GBuffer");
	auto temporalFilterMapVolume = (TemporalFilterMapVolume*) _temporalFilterMapVolume;

	PipelineAttribute frustumJitter;
	PipelineAttribute reprojectionMatrix;

	frustumJitter.type = PipelineAttribute::AttrType::ATTR_2F;
	reprojectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	frustumJitter.name = "frustumJitter";
	reprojectionMatrix.name = "reprojectionMatrix";

	frustumJitter.value = glm::vec3 (gBuffer->GetFrustumJitter (), 0.0f);

	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	glm::mat4 reprojectionMat = screenMatrix * temporalFilterMapVolume->GetViewProjectionMatrix () * glm::inverse (viewMatrix);

	reprojectionMatrix.matrix = reprojectionMat;

	attributes.push_back (frustumJitter);
	attributes.push_back (reprojectionMatrix);

	return attributes;
}

void TemporalFilterRenderPass::UpdateLastPostProcessMapVolume (const RenderSettings& settings)
{
	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	glm::ivec2 temporalFilterMapSize = _temporalFilterMapVolume->GetSize ();

	if (temporalFilterMapSize != volumeResolution) {

		/*
		 * Clear temporal filtering map volume
		*/

		_temporalFilterMapVolume->Clear ();

		/*
		 * Initialize temporal filtering map volume
		*/

		InitLastPostProcessMapVolume (settings);
	}
}

void TemporalFilterRenderPass::InitLastPostProcessMapVolume (const RenderSettings& settings)
{
	/*
	 * Initialize temporal anti-aliasing map volume
	*/

	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	if (!_temporalFilterMapVolume->Init (volumeResolution)) {
		Console::LogError (std::string () + "Temporal anti-aliasing volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}
