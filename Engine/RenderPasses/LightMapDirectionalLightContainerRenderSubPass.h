#ifndef LIGHTMAPDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H
#define LIGHTMAPDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"

#include <string>

class LightMapDirectionalLightContainerRenderSubPass : public DirectionalVolumetricLightContainerRenderSubPass
{
protected:
	std::string _shaderName;

public:
	LightMapDirectionalLightContainerRenderSubPass ();

	void Init ();

	void Clear ();
protected:
	void LockShader (const VolumetricLight*);

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif