#ifndef FORWARDRENDERPASS_H
#define FORWARDRENDERPASS_H

#include "Renderer/RenderPassI.h"

class ENGINE_API ForwardRenderPass : public RenderPassI
{
public:
	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartForwardPass (RenderVolumeCollection* rvc);
	void ForwardPass (const RenderScene*);
};

#endif