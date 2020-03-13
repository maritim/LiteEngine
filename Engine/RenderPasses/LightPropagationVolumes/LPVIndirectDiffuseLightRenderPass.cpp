#include "LPVIndirectDiffuseLightRenderPass.h"

#include "RenderPasses/IndirectDiffuseLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/LPVStatisticsObject.h"

bool LPVIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure light propagation volumes indirect light render pass
	*/

	return true;
}

std::string LPVIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesIndirectDiffuseFragment.glsl";
}

std::string LPVIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 LPVIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("LPVStatisticsObject");
		LPVStatisticsObject* lpvStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new LPVStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("LPVStatisticsObject", stat);
		}

		lpvStatisticsObject = dynamic_cast<LPVStatisticsObject*> (stat);

		lpvStatisticsObject->lpvIndirectDiffuseMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* LPVIndirectDiffuseLightRenderPass::CreatePostProcessVolume () const
{
	IndirectDiffuseLightMapVolume* indirectDiffuseLightMapVolume = new IndirectDiffuseLightMapVolume ();

	return indirectDiffuseLightMapVolume;
}

std::vector<PipelineAttribute> LPVIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute lpvIntensity;

	lpvIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lpvIntensity.name = "lpvIntensity";

	lpvIntensity.value.x = settings.lpv_intensity;

	attributes.push_back (lpvIntensity);

	return attributes;
}
