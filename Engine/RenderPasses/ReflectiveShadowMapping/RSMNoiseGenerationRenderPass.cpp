#include "RSMNoiseGenerationRenderPass.h"

// #include "Debug/Statistics/StatisticsManager.h"
// #include "Debug/Statistics/SSAOStatisticsObject.h"

bool RSMNoiseGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.rsm_noise_enabled;
}

glm::ivec2 RSMNoiseGenerationRenderPass::GetNoiseMapVolumeResolution (const RenderSettings& settings) const
{
	// if (_noiseMapVolume != nullptr) {
	// 	StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSAOStatisticsObject");
	// 	SSAOStatisticsObject* ssaoStatisticsObject = nullptr;

	// 	if (stat == nullptr) {
	// 		stat = new SSAOStatisticsObject ();
	// 		StatisticsManager::Instance ()->SetStatisticsObject ("SSAOStatisticsObject", stat);
	// 	}

	// 	ssaoStatisticsObject = dynamic_cast<SSAOStatisticsObject*> (stat);

	// 	ssaoStatisticsObject->noiseMapID = _noiseMapVolume->GetColorBuffer ();
	// }

	return glm::ivec2 (settings.rsm_noise_size, settings.rsm_noise_size);
}
