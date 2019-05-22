#ifndef VOLUMETRICLIGHT_H
#define VOLUMETRICLIGHT_H

#include "Light.h"

#include "Core/Resources/Resource.h"
#include "Mesh/Model.h"

#include "LightRenderer.h"

class VolumetricLight : public Light
{
protected:
	Resource<Model> _volume;
	LightRenderer* _lightRenderer;

public:
	VolumetricLight ();
	virtual ~VolumetricLight ();

	virtual LightRenderer* GetLightRenderer ();

	void SetVolume (const Resource<Model>& volume);
	Resource<Model> GetVolume ();

	virtual void Update () = 0;
};

#endif