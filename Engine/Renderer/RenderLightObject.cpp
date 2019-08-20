#include "RenderLightObject.h"

#include "Renderer/Pipeline.h"

RenderLightObject::RenderLightObject () :
	_lightColor (Color::White),
	_lightIntensity (1.0f),
	_castShadows (false)
{

}

void RenderLightObject::Draw ()
{
	Pipeline::SetObjectTransform (_transform);

	Pipeline::UpdateMatrices (nullptr);

	Pipeline::SendCustomAttributes ("", GetCustomAttributes ());

	_modelView->Draw ();
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

bool RenderLightObject::IsCastingShadows () const
{
	return _castShadows;
}

std::vector<PipelineAttribute> RenderLightObject::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightPosition;
	PipelineAttribute lightColor;
	PipelineAttribute lightIntensity;

	lightPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	lightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	lightIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lightPosition.name = "lightPosition";
	lightColor.name = "lightColor";
	lightIntensity.name = "lightIntensity";

	lightPosition.value = _transform->GetPosition ();
	lightColor.value = _lightColor.ToVector3 ();
	lightIntensity.value.x = _lightIntensity;

	attributes.push_back (lightPosition);
	attributes.push_back (lightColor);
	attributes.push_back (lightIntensity);

	return attributes;
}
