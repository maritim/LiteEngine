#include "RSMIndirectDiffuseLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RSMStatisticsObject.h"

#include "Utils/Extensions/MathExtend.h"

bool RSMIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string RSMIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapIndirectDiffuseFragment.glsl";
}

std::string RSMIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 RSMIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* RSMIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create reflective shadow mapping indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("indirectDiffuseMap"));

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

	auto rsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

	rsmStatisticsObject->rsmIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> RSMIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute rsmResolution;
	PipelineAttribute rsmRadius;
	PipelineAttribute rsmIntensity;
	PipelineAttribute rsmInterpolationScale;
	PipelineAttribute rsmMinInterpolationDistance;
	PipelineAttribute rsmMinInterpolationAngle;
	PipelineAttribute rsmDebugInterpolation;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmMinInterpolationDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmMinInterpolationAngle.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmDebugInterpolation.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmResolution.name = "rsmResolution";
	rsmRadius.name = "rsmRadius";
	rsmIntensity.name = "rsmIntensity";
	rsmInterpolationScale.name = "rsmInterpolationScale";
	rsmMinInterpolationDistance.name = "rsmMinInterpolationDistance";
	rsmMinInterpolationAngle.name = "rsmMinInterpolationAngle";
	rsmDebugInterpolation.name = "rsmDebugInterpolation";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmRadius.value.x = settings.rsm_radius;
	rsmIntensity.value.x = settings.rsm_intensity;
	rsmInterpolationScale.value.x = settings.rsm_interpolation_scale;
	rsmMinInterpolationDistance.value.x = settings.rsm_min_interpolation_distance;
	rsmMinInterpolationAngle.value.x = std::cos (DEG2RAD * settings.rsm_min_interpolation_angle);
	rsmDebugInterpolation.value.x = settings.rsm_debug_interpolation;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmRadius);
	attributes.push_back (rsmIntensity);
	attributes.push_back (rsmInterpolationScale);
	attributes.push_back (rsmMinInterpolationDistance);
	attributes.push_back (rsmMinInterpolationAngle);
	attributes.push_back (rsmDebugInterpolation);

	return attributes;
}
