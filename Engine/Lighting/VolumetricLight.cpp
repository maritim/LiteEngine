#include "VolumetricLight.h"

VolumetricLight::VolumetricLight () :
	_renderLightObject (nullptr)
{

}

VolumetricLight::~VolumetricLight ()
{
	delete _renderLightObject;
}

void VolumetricLight::SetActive (bool isActive)
{
	SceneObject::SetActive (isActive);

	_renderLightObject->SetActive (isActive);
}

void VolumetricLight::SetColor (const Color& color)
{
	Light::SetColor (color);

	_renderLightObject->SetLightColor (color);
}

void VolumetricLight::SetIntensity (float intensity)
{
	Light::SetIntensity (intensity);

	_renderLightObject->SetLightIntensity (intensity);
}

void VolumetricLight::SetShadowCasting (bool castShadows)
{
	Light::SetShadowCasting (castShadows);

	_renderLightObject->SetShadowCasting (castShadows);
}
