#include "RenderPointLightObject.h"

RenderPointLightObject::RenderPointLightObject () :
	_lightRange (0.0f)
{

}

void RenderPointLightObject::SetLightRange (float lightRange)
{
	_lightRange = lightRange;
}

float RenderPointLightObject::GetLightRange () const
{
	return _lightRange;
}
