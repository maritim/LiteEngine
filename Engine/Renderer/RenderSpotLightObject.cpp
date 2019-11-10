#include "RenderSpotLightObject.h"

RenderSpotLightObject::RenderSpotLightObject () :
	_lightSpotCutoff (0.0f),
	_lightSpotOuterCutoff (0.0f)
{

}

void RenderSpotLightObject::SetLightSpotCutoff (float lightSpotCutoff)
{
	_lightSpotCutoff = lightSpotCutoff;
}

void RenderSpotLightObject::SetLightSpotOuterCutoff (float lightSpotOuterCutoff)
{
	_lightSpotOuterCutoff = lightSpotOuterCutoff;
}

float RenderSpotLightObject::GetLightSpotCutoff () const
{
	return _lightSpotCutoff;
}

float RenderSpotLightObject::GetLightSpotOuterCutoff () const
{
	return _lightSpotOuterCutoff;
}
