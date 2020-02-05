#ifndef SSDODIRECTIONALLIGHTRENDERPASS_H
#define SSDODIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class SSDODirectionalLightRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(SSDODirectionalLightRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif