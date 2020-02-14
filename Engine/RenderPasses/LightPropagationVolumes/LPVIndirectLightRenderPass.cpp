#include "LPVIndirectLightRenderPass.h"

#include "RenderPasses/IndirectLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/LPVStatisticsObject.h"

bool LPVIndirectLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure light propagation volumes indirect light render pass
	*/

	return true;
}

std::string LPVIndirectLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesIndirectFragment.glsl";
}

std::string LPVIndirectLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectMap";
}

glm::ivec2 LPVIndirectLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("LPVStatisticsObject");
		LPVStatisticsObject* lpvStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new LPVStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("LPVStatisticsObject", stat);
		}

		lpvStatisticsObject = dynamic_cast<LPVStatisticsObject*> (stat);

		lpvStatisticsObject->lpvIndirectMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* LPVIndirectLightRenderPass::CreatePostProcessVolume () const
{
	IndirectLightMapVolume* indirectLightMapVolume = new IndirectLightMapVolume ();

	return indirectLightMapVolume;
}

std::vector<PipelineAttribute> LPVIndirectLightRenderPass::GetCustomAttributes (const Camera* camera,
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
