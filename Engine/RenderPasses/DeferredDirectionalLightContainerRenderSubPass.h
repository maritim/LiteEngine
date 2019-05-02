#ifndef DEFERREDDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H
#define DEFERREDDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"

#include <string>

class DeferredDirectionalLightContainerRenderSubPass : public DirectionalVolumetricLightContainerRenderSubPass
{
protected:
	std::string _shaderName;
	std::string _shadowShaderName;

public:
	DeferredDirectionalLightContainerRenderSubPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const VolumetricLight*);

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif