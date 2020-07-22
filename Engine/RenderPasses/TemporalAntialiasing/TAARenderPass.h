#ifndef TAARENDERPASS_H
#define TAARENDERPASS_H

#include "RenderPasses/TemporalFiltering/TemporalFilterRenderPass.h"

#define TEMPORAL_ANTI_ALIASING_MAP_VOLUME_NOT_INIT 354

class ENGINE_API TAARenderPass : public TemporalFilterRenderPass
{
	DECLARE_RENDER_PASS(TAARenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
};

#endif