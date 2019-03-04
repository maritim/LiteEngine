#include "Light.h"

Light::Light () :
	_color (Color::White),
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

bool Light::IsCastingShadows () const
{
	return _castShadows;
}

void Light::SetColor (const Color& color)
{
	_color = color;
}

void Light::SetShadowCasting (bool castShadows)
{
	_castShadows = castShadows;
}
