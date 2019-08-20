#include "RSMVolume.h"

#include <glm/gtc/matrix_transform.hpp>

void RSMVolume::SetLightCamera (Camera* camera)
{
	_camera = camera;
}

Camera* RSMVolume::GetLightCamera ()
{
	return _camera;
}

void RSMVolume::BindForReading ()
{
	/*
	 * Bind for reading first shadow map cascade
	*/

	RSMDirectionalLightVolume::BindForReading ();
}

void RSMVolume::BindForWriting ()
{
	/*
	 * Bind for writing on first cascade shadow map
	*/

	RSMDirectionalLightVolume::BindForShadowMapCatch (0);
}

std::vector<PipelineAttribute> RSMVolume::GetCustomAttributes () const
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

	reflectiveShadowMapDepth.value.x = 15;
	reflectiveShadowMapPos.value.x = 16;
	reflectiveShadowMapNormal.value.x = 17;
	reflectiveShadowMapFlux.value.x = 18;

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
