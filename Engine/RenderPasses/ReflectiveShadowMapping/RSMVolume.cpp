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

	reflectiveShadowMapDepth.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	reflectiveShadowMapPos.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	reflectiveShadowMapNormal.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	reflectiveShadowMapFlux.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	reflectiveShadowMapDepth.name = "rsmShadowMap";
	reflectiveShadowMapPos.name = "rsmPositionMap";
	reflectiveShadowMapNormal.name = "rsmNormalMap";
	reflectiveShadowMapFlux.name = "rsmFluxMap";
	lightSpaceMatrix.name = "rsmLightSpaceMatrix";

	reflectiveShadowMapDepth.value.x = _shadowMapBuffers [0]->GetDepthBuffer ();
	reflectiveShadowMapPos.value.x = _shadowMapBuffers [0]->GetColorBuffer (0);
	reflectiveShadowMapNormal.value.x = _shadowMapBuffers [0]->GetColorBuffer (1);
	reflectiveShadowMapFlux.value.x = _shadowMapBuffers [0]->GetColorBuffer (2);

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
