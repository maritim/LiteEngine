#include "RSMAmbientOcclusionRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RSMStatisticsObject.h"

bool RSMAmbientOcclusionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string RSMAmbientOcclusionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAmbientOcclusionFragment.glsl";
}

std::string RSMAmbientOcclusionRenderPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 RSMAmbientOcclusionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {

	}

	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* RSMAmbientOcclusionRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create rsm ambient occlusion framebuffer
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

	auto rsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

	rsmStatisticsObject->rsmAmbientOcclusionMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> RSMAmbientOcclusionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;

	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";

	ssaoRadius.value.x = settings.ssao_radius;
	ssaoBias.value.x = settings.ssao_bias;

	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);

	return attributes;
}
