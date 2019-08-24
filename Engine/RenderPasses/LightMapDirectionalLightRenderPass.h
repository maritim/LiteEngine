#ifndef LIGHTMAPDIRECTIONALLIGHTRENDERPASS_H
#define LIGHTMAPDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include <string>

class LightMapDirectionalLightRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(LightMapDirectionalLightRenderPass)

protected:
	std::string _shaderName;

public:
	LightMapDirectionalLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif