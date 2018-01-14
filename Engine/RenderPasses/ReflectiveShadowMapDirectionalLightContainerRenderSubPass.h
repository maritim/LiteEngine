#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"

#include <string>

class ReflectiveShadowMapDirectionalLightContainerRenderSubPass : public DirectionalVolumetricLightContainerRenderSubPass
{
protected:
	std::string _shadowShaderName;

public:
	ReflectiveShadowMapDirectionalLightContainerRenderSubPass ();

	void Init ();
protected:
	void LockShader (const VolumetricLight*);
};

#endif