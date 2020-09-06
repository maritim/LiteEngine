#include "LPVAmbientOcclusionRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "LPVStatisticsObject.h"

bool LPVAmbientOcclusionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string LPVAmbientOcclusionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesAmbientOcclusionFragment.glsl";
}

std::string LPVAmbientOcclusionRenderPass::GetPostProcessVolumeName () const
{
	return "LPVAOVolume";
}

glm::ivec2 LPVAmbientOcclusionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* LPVAmbientOcclusionRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("ambientOcclusionMap"));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_R8);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RED);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_UNSIGNED_BYTE);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	FramebufferRenderVolume* renderVolume = new FramebufferRenderVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto lpvStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <LPVStatisticsObject> ();

	lpvStatisticsObject->lpvAmbientOcclusionMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> LPVAmbientOcclusionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/



	return attributes;
}
