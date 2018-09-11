#include "Light.h"

Light::Light () :
	_color (Color::White),
	_specularColor (Color::White),
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

Color Light::GetSpecularColor () const
{
	return _specularColor;
}

bool Light::IsCastingShadows () const
{
	return _castShadows;
}

void Light::SetColor (const Color& color)
{
	_color = color;
}

void Light::SetSpecularColor (const Color& specularColor)
{
	_specularColor = specularColor;
}

void Light::SetShadowCasting (bool castShadows)
{
	_castShadows = castShadows;
}
