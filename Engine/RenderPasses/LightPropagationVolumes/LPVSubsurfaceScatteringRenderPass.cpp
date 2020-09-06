#include "LPVSubsurfaceScatteringRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "LPVStatisticsObject.h"

bool LPVSubsurfaceScatteringRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure light propagation volumes indirect light render pass
	*/

	return true;
}

std::string LPVSubsurfaceScatteringRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesSubsurfaceScatteringFragment.glsl";
}

std::string LPVSubsurfaceScatteringRenderPass::GetPostProcessVolumeName () const
{
	return "SubsurfaceScatteringMap";
}

glm::ivec2 LPVSubsurfaceScatteringRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* LPVSubsurfaceScatteringRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create subsurface scattering framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("subsurfaceScatteringMap"));

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

	lpvStatisticsObject->lpvSubsurfaceScatteringMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> LPVSubsurfaceScatteringRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute lpvIndirectRefractiveIntensity;

	lpvIndirectRefractiveIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lpvIndirectRefractiveIntensity.name = "lpvIndirectRefractiveIntensity";

	lpvIndirectRefractiveIntensity.value.x = settings.lpv_indirect_refractive_intensity;

	attributes.push_back (lpvIndirectRefractiveIntensity);

	return attributes;
}
