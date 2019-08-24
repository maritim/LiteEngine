#ifndef DEFERREDPOINTLIGHTRENDERPASS_H
#define DEFERREDPOINTLIGHTRENDERPASS_H

#include "RenderPasses/PointVolumetricLightRenderPass.h"

#include <string>

class ENGINE_API DeferredPointLightRenderPass : public PointVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DeferredPointLightRenderPass)

protected:
	std::string _shaderName;
	std::string _shadowShaderName;

public:
	DeferredPointLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);
};

#endif