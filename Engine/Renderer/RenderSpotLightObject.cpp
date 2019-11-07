#include "RenderSpotLightObject.h"

#include "Utils/Extensions/MathExtend.h"

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

std::vector<PipelineAttribute> RenderSpotLightObject::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes = RenderPointLightObject::GetCustomAttributes ();

	PipelineAttribute lightDirection;
	PipelineAttribute lightSpotCutoff;
	PipelineAttribute lightSpotOuterCutoff;

	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	lightSpotCutoff.type = PipelineAttribute::AttrType::ATTR_1F;
	lightSpotOuterCutoff.type = PipelineAttribute::AttrType::ATTR_1F;

	lightDirection.name = "lightDirection";
	lightSpotCutoff.name = "lightSpotCutoff";
	lightSpotOuterCutoff.name = "lightSpotOuterCutoff";

	glm::vec3 lightDir = _transform->GetRotation () * glm::vec3 (-1, 0, 0);

	lightDirection.value = lightDir;
	lightSpotCutoff.value.x = std::cos (DEG2RAD * _lightSpotCutoff);
	lightSpotOuterCutoff.value.x = std::cos (DEG2RAD * _lightSpotOuterCutoff);

	attributes.push_back (lightDirection);
	attributes.push_back (lightSpotCutoff);
	attributes.push_back (lightSpotOuterCutoff);

	return attributes;
}
