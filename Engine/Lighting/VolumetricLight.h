#ifndef VOLUMETRICLIGHT_H
#define VOLUMETRICLIGHT_H

#include "Light.h"

#include "Mesh/Model.h"

#include "LightRenderer.h"

class VolumetricLight : public Light
{
protected:
	Model* _volume;
	LightRenderer* _lightRenderer;

public:
	VolumetricLight ();
	virtual ~VolumetricLight ();

	virtual LightRenderer* GetLightRenderer ();

	void SetVolume (Model* volume);
	Model* GetVolume ();

	virtual void Update () = 0;
};

#endif