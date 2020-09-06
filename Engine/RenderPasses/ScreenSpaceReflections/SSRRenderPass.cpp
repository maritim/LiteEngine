#include "SSRRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSRStatisticsObject.h"

bool SSRRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space reflection is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflections/screenSpaceReflectionFragment.glsl";
}

std::string SSRRenderPass::GetPostProcessVolumeName () const
{
	return "SSRMapVolume";
}

glm::ivec2 SSRRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssr_scale);
}

FramebufferRenderVolume* SSRRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create screen space reflections framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("reflectionMap"));

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

	auto ssrStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSRStatisticsObject> ();

	ssrStatisticsObject->ssrPositionMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSRRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrResolution;
	PipelineAttribute ssrIterations;
	PipelineAttribute ssrRoughness;
	PipelineAttribute ssrThickness;
	PipelineAttribute ssrStride;

	ssrResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssrIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	ssrRoughness.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrStride.type = PipelineAttribute::AttrType::ATTR_1I;

	ssrResolution.name = "ssrResolution";
	ssrIterations.name = "ssrIterations";
	ssrRoughness.name = "ssrRoughness";
	ssrThickness.name = "ssrThickness";
	ssrStride.name = "ssrStride";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssr_scale);

	ssrResolution.value = glm::vec3 (resolution, 0.0f);
	ssrIterations.value.x = settings.ssr_iterations;
	ssrRoughness.value.x = settings.ssr_roughness;
	ssrThickness.value.x = settings.ssr_thickness;
	ssrStride.value.x = settings.ssr_stride;

	attributes.push_back (ssrResolution);
	attributes.push_back (ssrIterations);
	attributes.push_back (ssrRoughness);
	attributes.push_back (ssrThickness);
	attributes.push_back (ssrStride);

	return attributes;
}
