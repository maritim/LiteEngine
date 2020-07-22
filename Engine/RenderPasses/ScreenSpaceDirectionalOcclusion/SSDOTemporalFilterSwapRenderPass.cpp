#include "SSDOTemporalFilterSwapRenderPass.h"

bool SSDOTemporalFilterSwapRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if temporal anti-aliasing is enabled
	*/

	return settings.ssdo_enabled && settings.ssdo_temporal_filter_enabled;
}

std::string SSDOTemporalFilterSwapRenderPass::GetPostProcessMapVolumeName () const
{
	return "SSDOMapVolume";
}
