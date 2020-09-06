#include "VCTAmbientOcclusionRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "VCTStatisticsObject.h"

bool VCTAmbientOcclusionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string VCTAmbientOcclusionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingAmbientOcclusionFragment.glsl";
}

std::string VCTAmbientOcclusionRenderPass::GetPostProcessVolumeName () const
{
	return "VCTAOMap";
}

glm::ivec2 VCTAmbientOcclusionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* VCTAmbientOcclusionRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create ambient occlusion framebuffer
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

	auto vctStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <VCTStatisticsObject> ();

	vctStatisticsObject->vctAmbientOcclusionMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> VCTAmbientOcclusionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute originBias;

	originBias.type = PipelineAttribute::AttrType::ATTR_1F;

	originBias.name = "originBias";

	originBias.value.x = settings.vct_origin_bias;

	attributes.push_back (originBias);

	return attributes;
}
