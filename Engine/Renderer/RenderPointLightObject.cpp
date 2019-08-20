#include "RenderPointLightObject.h"

RenderPointLightObject::RenderPointLightObject () :
	_attenuation (0, 0, 1)
{

}

void RenderPointLightObject::SetLightAttenuation (const glm::vec3& attenuation)
{
	_attenuation = attenuation;
}

std::vector<PipelineAttribute> RenderPointLightObject::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = RenderLightObject::GetCustomAttributes ();

	PipelineAttribute attenuation;

	attenuation.type = PipelineAttribute::AttrType::ATTR_3F;

	attenuation.name = "attenuationComp";

	attenuation.value = _attenuation;

	attributes.push_back (attenuation);

	return attributes;
}
