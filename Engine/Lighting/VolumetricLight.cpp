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

void VolumetricLight::SetShadow (const Light::Shadow& shadow)
{
	Light::SetShadow (shadow);

	RenderLightObject::Shadow lightShadow;

	lightShadow.resolution = shadow.resolution;
	lightShadow.cascadesCount = shadow.cascadesCount;
	lightShadow.bias = shadow.bias;

	_renderLightObject->SetLightShadow (lightShadow);
}
