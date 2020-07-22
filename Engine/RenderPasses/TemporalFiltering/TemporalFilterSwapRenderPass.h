#ifndef TEMPORALFILTERSWAPRENDERPASS_H
#define TEMPORALFILTERSWAPRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class ENGINE_API TemporalFilterSwapRenderPass : public ContainerRenderSubPassI
{
public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	virtual std::string GetPostProcessMapVolumeName () const;
};

#endif