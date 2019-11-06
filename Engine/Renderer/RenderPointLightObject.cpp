#include "RenderPointLightObject.h"

RenderPointLightObject::RenderPointLightObject () :
	_lightRange (0.0f)
{

}

void RenderPointLightObject::SetLightRange (float lightRange)
{
	_lightRange = lightRange;
}

std::vector<PipelineAttribute> RenderPointLightObject::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes = RenderLightObject::GetCustomAttributes ();

	PipelineAttribute lightRange;

	lightRange.type = PipelineAttribute::AttrType::ATTR_1F;

	lightRange.name = "lightRange";

	lightRange.value.x = _lightRange;

	attributes.push_back (lightRange);

	return attributes;
}
