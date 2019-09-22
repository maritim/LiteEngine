#include "SSAONoiseGenerationRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/SSAOStatisticsObject.h"

bool SSAONoiseGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

glm::ivec2 SSAONoiseGenerationRenderPass::GetNoiseMapVolumeResolution (const RenderSettings& settings) const
{
	if (_noiseMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSAOStatisticsObject");
		SSAOStatisticsObject* ssaoStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new SSAOStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("SSAOStatisticsObject", stat);
		}

		ssaoStatisticsObject = dynamic_cast<SSAOStatisticsObject*> (stat);

		ssaoStatisticsObject->noiseMapID = _noiseMapVolume->GetColorBuffer ();
	}

	return glm::ivec2 (settings.ssao_noise_size, settings.ssao_noise_size);
}
