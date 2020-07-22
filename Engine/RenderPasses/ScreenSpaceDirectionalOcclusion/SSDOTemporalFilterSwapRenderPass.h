#ifndef SSDOTEMPORALFILTERSWAPRENDERPASS_H
#define SSDOTEMPORALFILTERSWAPRENDERPASS_H

#include "RenderPasses/TemporalFiltering/TemporalFilterSwapRenderPass.h"

class ENGINE_API SSDOTemporalFilterSwapRenderPass : public TemporalFilterSwapRenderPass
{
	DECLARE_RENDER_PASS(SSDOTemporalFilterSwapRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	virtual std::string GetPostProcessMapVolumeName () const;
};

#endif