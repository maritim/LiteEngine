#ifndef RSMDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H
#define RSMDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"

#include <string>

class RSMDirectionalLightContainerRenderSubPass : public DirectionalVolumetricLightContainerRenderSubPass
{
protected:
	std::string _shadowShaderName;

public:
	RSMDirectionalLightContainerRenderSubPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void DirectionalLightPass (const RenderScene*, const Camera*, RenderVolumeCollection*);

	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif