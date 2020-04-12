#include "LightComponent.h"

LightComponent::ShadowInformation::ShadowInformation () :
	resolution (0),
	cascadesCount (1),
	bias (0.0f)
{

}

LightComponent::LightComponent () :
	_color (Color::White),
	_intensity (1.0f),
	_castShadows (false),
	_renderLightObject (nullptr)
{

}

LightComponent::~LightComponent ()
{
	delete _renderLightObject;
}

void LightComponent::SetActive (bool isActive)
{
	_renderLightObject->SetActive (isActive);
}

Color LightComponent::GetColor () const
{
	return _color;
}

float LightComponent::GetIntensity () const
{
	return _intensity;
}

bool LightComponent::IsCastingShadows () const
{
	return _castShadows;
}

LightComponent::ShadowInformation LightComponent::GetShadowInformation () const
{
	return _shadowInformation;
}

void LightComponent::SetColor (const Color& color)
{
	_color = color;

	_renderLightObject->SetLightColor (color);
}

void LightComponent::SetIntensity (float intensity)
{
	_intensity = intensity;

	_renderLightObject->SetLightIntensity (intensity);
}

void LightComponent::SetShadowCasting (bool castShadows)
{
	_castShadows = castShadows;

	_renderLightObject->SetShadowCasting (_castShadows);
}

void LightComponent::SetShadowInformation (const LightComponent::ShadowInformation& shadow)
{
	_shadowInformation = shadow;

	RenderLightObject::Shadow lightShadow;

	lightShadow.resolution = _shadowInformation.resolution;
	lightShadow.cascadesCount = _shadowInformation.cascadesCount;
	lightShadow.bias = _shadowInformation.bias;

	_renderLightObject->SetLightShadow (lightShadow);
}
