#include "RenderLightObject.h"

#include "Renderer/Pipeline.h"

RenderLightObject::Shadow::Shadow () :
	resolution (0),
	cascadesCount (1),
	bias (0)
{

}

RenderLightObject::RenderLightObject () :
	_lightColor (Color::White),
	_lightIntensity (1.0f),
	_castShadows (false)
{

}

void RenderLightObject::SetLightColor (const Color& color)
{
	_lightColor = color;
}

void RenderLightObject::SetLightIntensity (float intensity)
{
	_lightIntensity = intensity;
}

void RenderLightObject::SetShadowCasting (bool castShadows)
{
	_castShadows = castShadows;
}

void RenderLightObject::SetLightShadow (const RenderLightObject::Shadow& lightShadow)
{
	_lightShadow = lightShadow;
}

Color RenderLightObject::GetLightColor () const
{
	return _lightColor;
}

float RenderLightObject::GetLightIntensity () const
{
	return _lightIntensity;
}

bool RenderLightObject::IsCastingShadows () const
{
	return _castShadows;
}

RenderLightObject::Shadow RenderLightObject::GetShadow () const
{
	return _lightShadow;
}
