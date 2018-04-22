#include "ReflectiveShadowMapVolume.h"

#include <glm/gtc/matrix_transform.hpp>

void ReflectiveShadowMapVolume::SetLightCamera (Camera* camera)
{
	_camera = camera;
}

Camera* ReflectiveShadowMapVolume::GetLightCamera ()
{
	return _camera;
}

void ReflectiveShadowMapVolume::BindForReading ()
{
	/*
	 * Bind for reading first shadow map cascade
	*/

	ReflectiveShadowMapDirectionalLightVolume::BindForReading ();
}

void ReflectiveShadowMapVolume::BindForWriting ()
{
	/*
	 * Bind for writing on first cascade shadow map
	*/

	ReflectiveShadowMapDirectionalLightVolume::BindForShadowMapCatch (0);
}

std::vector<PipelineAttribute> ReflectiveShadowMapVolume::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute reflectiveShadowMapDepth;
	PipelineAttribute reflectiveShadowMapPos;
	PipelineAttribute reflectiveShadowMapNormal;
	PipelineAttribute reflectiveShadowMapFlux;
	PipelineAttribute lightSpaceMatrix;

	reflectiveShadowMapDepth.type = PipelineAttribute::AttrType::ATTR_1I;
	reflectiveShadowMapPos.type = PipelineAttribute::AttrType::ATTR_1I;
	reflectiveShadowMapNormal.type = PipelineAttribute::AttrType::ATTR_1I;
	reflectiveShadowMapFlux.type = PipelineAttribute::AttrType::ATTR_1I;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	reflectiveShadowMapDepth.name = "rsmShadowMap";
	reflectiveShadowMapPos.name = "rsmPositionMap";
	reflectiveShadowMapNormal.name = "rsmNormalMap";
	reflectiveShadowMapFlux.name = "rsmFluxMap";
	lightSpaceMatrix.name = "rsmLightSpaceMatrix";

	reflectiveShadowMapDepth.value.x = 8;
	reflectiveShadowMapPos.value.x = 9;
	reflectiveShadowMapNormal.value.x = 10;
	reflectiveShadowMapFlux.value.x = 11;

	glm::mat4 lightProjection = _camera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (_camera->GetRotation ()), _camera->GetPosition () * -1.0f);

	lightSpaceMatrix.matrix = lightProjection * lightView;

	attributes.push_back (reflectiveShadowMapDepth);
	attributes.push_back (reflectiveShadowMapPos);
	attributes.push_back (reflectiveShadowMapNormal);
	attributes.push_back (reflectiveShadowMapFlux);
	attributes.push_back (lightSpaceMatrix);

	return attributes;
}
