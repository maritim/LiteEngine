#include "PointLightRenderer.h"

#include "Managers/ShaderManager.h"

#include "PointLight.h"

PointLightRenderer::PointLightRenderer (Light* light) :
	LightRenderer (light)
{
	_shaderName = "POINT_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredPointVolLightVertex.glsl",
		"Assets/Shaders/deferredPointVolLightFragment.glsl");
}

std::vector<PipelineAttribute> PointLightRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = LightRenderer::GetCustomAttributes ();

	return attributes;
	// std::vector<PipelineAttribute> attributes;

	// PipelineAttribute lightPosition;
	// PipelineAttribute lightColor;
	// PipelineAttribute lightSpecularColor;

	// lightPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	// lightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	// lightSpecularColor.type = PipelineAttribute::AttrType::ATTR_3F;

	// lightPosition.name = "lightPosition";
	// lightColor.name = "lightColor";
	// lightSpecularColor = "lightSpecularColor";

	// lightPosition.value = _transform->GetPosition ();
	// lightColor.value = _light->GetColor ().ToVector3 ();
	// lightSpecularColor.value = _light->GetSpecularColor ().ToVector3 ();

	// attributes.push_back (lightPosition;
	// attributes.push_back (lightColor);
	// attributes.push_back (lightSpecularColor);
} 