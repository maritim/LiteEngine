#include "VCTSubsurfaceScatteringRenderPass.h"

#include "RenderPasses/SubsurfaceScatteringMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/VCTStatisticsObject.h"

bool VCTSubsurfaceScatteringRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string VCTSubsurfaceScatteringRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingSubsurfaceScatteringFragment.glsl";
}

std::string VCTSubsurfaceScatteringRenderPass::GetPostProcessVolumeName () const
{
	return "VCTSSSMap";
}

glm::ivec2 VCTSubsurfaceScatteringRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("VCTStatisticsObject");
		VCTStatisticsObject* vctStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new VCTStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("VCTStatisticsObject", stat);
		}

		vctStatisticsObject = dynamic_cast<VCTStatisticsObject*> (stat);

		vctStatisticsObject->vctSubsurfaceScatteringMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* VCTSubsurfaceScatteringRenderPass::CreatePostProcessVolume () const
{
	SubsurfaceScatteringMapVolume* vctsssMapVolume = new SubsurfaceScatteringMapVolume ();

	return vctsssMapVolume;
}

std::vector<PipelineAttribute> VCTSubsurfaceScatteringRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute refractiveIndirectIntensity;
	PipelineAttribute refractiveConeRatio;
	PipelineAttribute refractiveConeDistance;

	refractiveIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;

	refractiveIndirectIntensity.name = "refractiveIndirectIntensity";
	refractiveConeRatio.name = "refractiveConeRatio";
	refractiveConeDistance.name = "refractiveConeDistance";

	refractiveIndirectIntensity.value.x = settings.vct_indirect_refractive_intensity;
	refractiveConeRatio.value.x = settings.vct_refractive_cone_ratio;
	refractiveConeDistance.value.x = settings.vct_refractive_cone_distance;

	attributes.push_back (refractiveIndirectIntensity);
	attributes.push_back (refractiveConeRatio);
	attributes.push_back (refractiveConeDistance);

	return attributes;
}
