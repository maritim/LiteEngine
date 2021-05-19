#include "LPVIndirectSpecularLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "LPVStatisticsObject.h"

bool LPVIndirectSpecularLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure light propagation volumes indirect light render pass
	*/

	return true;
}

std::string LPVIndirectSpecularLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesIndirectSpecularFragment.glsl";
}

std::string LPVIndirectSpecularLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectSpecularMap";
}

glm::ivec2 LPVIndirectSpecularLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* LPVIndirectSpecularLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create indirect specular light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("indirectSpecularMap"));

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

	FramebufferRenderVolume* renderVolume = new FramebufferRenderVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto lpvStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <LPVStatisticsObject> ();

	lpvStatisticsObject->lpvIndirectSpecularMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> LPVIndirectSpecularLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute lpvSpecularIntensity;
	PipelineAttribute lpvSpecularIterations;

	lpvSpecularIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	lpvSpecularIterations.type = PipelineAttribute::AttrType::ATTR_1I;

	lpvSpecularIntensity.name = "lpvSpecularIntensity";
	lpvSpecularIterations.name = "lpvSpecularIterations";

	lpvSpecularIntensity.value.x = settings.lpv_indirect_specular_intensity;
	lpvSpecularIterations.value.x = settings.lpv_specular_iterations;

	attributes.push_back (lpvSpecularIntensity);
	attributes.push_back (lpvSpecularIterations);

	return attributes;
}
