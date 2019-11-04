#include "Light.h"

Light::Shadow::Shadow () :
	resolution (0),
	cascadesCount (1),
	bias (0.0f)
{

}

Light::Light () :
	_color (Color::White),
	_intensity (1.0f),
	_castShadows (false)
{

}

Light::~Light ()
{

}

Color Light::GetColor () const
{
	return _color;
}

float Light::GetIntensity () const
{
	return _intensity;
}

bool Light::IsCastingShadows () const
{
	return _castShadows;
}

Light::Shadow Light::GetShadow () const
{
	return _shadow;
}

void Light::SetColor (const Color& color)
{
	_color = color;
}

void Light::SetIntensity (float intensity)
{
	_intensity = intensity;
}

void Light::SetShadowCasting (bool castShadows)
{
	_castShadows = castShadows;
}

void Light::SetShadow (const Light::Shadow& shadow)
{
	_shadow = shadow;
}