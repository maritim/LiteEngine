#ifndef DEFERREDDIRECTIONALVOLUMETRICLIGHTRENDERPASS_H
#define DEFERREDDIRECTIONALVOLUMETRICLIGHTRENDERPASS_H

#include "DirectionalVolumetricLightRenderPass.h"

#include <string>

class DeferredDirectionalVolumetricLightRenderPass : public DirectionalVolumetricLightRenderPass
{
protected:
	std::string _shaderName;
	std::string _shadowShaderName;

public:
	DeferredDirectionalVolumetricLightRenderPass ();

	void Init ();
protected:
	void LockShader (const VolumetricLight*);
};

#endif