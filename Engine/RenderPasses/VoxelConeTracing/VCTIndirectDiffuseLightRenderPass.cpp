#include "VCTIndirectDiffuseLightRenderPass.h"

#include "RenderPasses/IndirectDiffuseLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/VCTStatisticsObject.h"

bool VCTIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string VCTIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingIndirectDiffuseFragment.glsl";
}

std::string VCTIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 VCTIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("VCTStatisticsObject");
		VCTStatisticsObject* vctStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new VCTStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("VCTStatisticsObject", stat);
		}

		vctStatisticsObject = dynamic_cast<VCTStatisticsObject*> (stat);

		vctStatisticsObject->vctIndirectDiffuseMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* VCTIndirectDiffuseLightRenderPass::CreatePostProcessVolume () const
{
	IndirectDiffuseLightMapVolume* indirectDiffuseLightMapVolume = new IndirectDiffuseLightMapVolume();

	return indirectDiffuseLightMapVolume;
}

std::vector<PipelineAttribute> VCTIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute vctIndirectIntensity;
	PipelineAttribute diffuseConeDistance;
	PipelineAttribute originBias;

	vctIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	diffuseConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	originBias.type = PipelineAttribute::AttrType::ATTR_1F;

	vctIndirectIntensity.name = "vctIndirectIntensity";
	diffuseConeDistance.name = "diffuseConeDistance";
	originBias.name = "originBias";

	vctIndirectIntensity.value.x = settings.vct_indirect_diffuse_intensity;
	diffuseConeDistance.value.x = settings.vct_diffuse_cone_distance;
	originBias.value.x = settings.vct_origin_bias;

	attributes.push_back (vctIndirectIntensity);
	attributes.push_back (diffuseConeDistance);
	attributes.push_back (originBias);

	return attributes;
}
