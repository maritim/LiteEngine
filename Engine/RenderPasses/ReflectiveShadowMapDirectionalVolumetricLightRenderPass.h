#ifndef REFLECTIVESHADOWMAPDIRECTIONALVOLUMETRICLIGHTRENDERPASS_H
#define REFLECTIVESHADOWMAPDIRECTIONALVOLUMETRICLIGHTRENDERPASS_H

#include "DirectionalVolumetricLightRenderPass.h"

#include <string>

class ReflectiveShadowMapDirectionalVolumetricLightRenderPass : public DirectionalVolumetricLightRenderPass
{
protected:
	std::string _shadowShaderName;

public:
	ReflectiveShadowMapDirectionalVolumetricLightRenderPass ();

	void Init ();
protected:
	void LockShader (const VolumetricLight*);
};

#endif