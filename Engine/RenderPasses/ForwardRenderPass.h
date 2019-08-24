#ifndef FORWARDRENDERPASS_H
#define FORWARDRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class ENGINE_API ForwardRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(ForwardRenderPass)

public:
	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void StartForwardPass (RenderVolumeCollection* rvc);
	void ForwardPass (const RenderScene*);
};

#endif