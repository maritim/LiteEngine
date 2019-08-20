#ifndef VOLUMETRICLIGHT_H
#define VOLUMETRICLIGHT_H

#include "Light.h"

#include "Renderer/RenderLightObject.h"

class VolumetricLight : public Light
{
protected:
	RenderLightObject* _renderLightObject;

public:
	VolumetricLight ();
	virtual ~VolumetricLight ();

	void SetActive (bool isActive);

	void SetColor (const Color& color);
	void SetIntensity (float intensity);
	void SetShadowCasting (bool castShadows);
};

#endif