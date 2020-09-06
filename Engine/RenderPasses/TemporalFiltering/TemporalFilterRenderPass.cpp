#include "TemporalFilterRenderPass.h"

#include "TemporalFilterMapVolume.h"

#include "RenderPasses/GBuffer.h"

#include "Core/Console/Console.h"

TemporalFilterRenderPass::TemporalFilterRenderPass () :
	_temporalFilterMapVolume (nullptr)
{

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

	_temporalFilterMapVolume = CreatePostProcessVolume (settings);

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

	delete _temporalFilterMapVolume;

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
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height));
}

FramebufferRenderVolume* TemporalFilterRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	Resource<Texture> texture = Resource<Texture> (new Texture (""));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	return new TemporalFilterMapVolume (framebuffer);
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

	auto framebufferSize = _temporalFilterMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if (framebufferSize.width != (std::size_t) volumeResolution.x ||
		framebufferSize.height != (std::size_t) volumeResolution.y) {

		/*
		 * Clear temporal filtering map volume
		*/

		delete _temporalFilterMapVolume;

		/*
		 * Initialize temporal filtering map volume
		*/

		_temporalFilterMapVolume = CreatePostProcessVolume (settings);
	}
}
