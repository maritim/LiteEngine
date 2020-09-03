#include "VCTAmbientOcclusionRenderPass.h"

#include "RenderPasses/AmbientOcclusion/AmbientOcclusionMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/VCTStatisticsObject.h"

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
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("VCTStatisticsObject");
		VCTStatisticsObject* vctStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new VCTStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("VCTStatisticsObject", stat);
		}

		vctStatisticsObject = dynamic_cast<VCTStatisticsObject*> (stat);

		vctStatisticsObject->vctAmbientOcclusionMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* VCTAmbientOcclusionRenderPass::CreatePostProcessVolume () const
{
	AmbientOcclusionMapVolume* vctaoMapVolume = new AmbientOcclusionMapVolume ();

	return vctaoMapVolume;
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
