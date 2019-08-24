#ifndef RSMDIRECTIONALLIGHTRENDERPASS_H
#define RSMDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include <string>

class RSMDirectionalLightRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(RSMDirectionalLightRenderPass)

protected:
	std::string _shadowShaderName;

public:
	RSMDirectionalLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void DirectionalLightPass (const RenderScene*, const Camera*, RenderVolumeCollection*);

	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif