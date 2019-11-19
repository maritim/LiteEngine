#include "PerspectiveShadowMapVolume.h"

PerspectiveShadowMapVolume::PerspectiveShadowMapVolume () :
	_lightCamera (nullptr),
	_shadowBias (0.0f)
{

}

bool PerspectiveShadowMapVolume::Init (PerspectiveCamera* lightCamera, const glm::ivec2& size)
{
	/*
	 * Keep light camera
	*/

	_lightCamera = lightCamera;

	/*
	 * Initialize shadow map volume
	*/

	return ShadowMapVolume::Init (size);
}

void PerspectiveShadowMapVolume::Clear ()
{
	/*
	 * Clear shadow map volume
	*/

	ShadowMapVolume::Clear ();

	/*
	 * Delete light camera
	*/

	delete _lightCamera;
}

void PerspectiveShadowMapVolume::SetShadowBias (float shadowBias)
{
	_shadowBias = shadowBias;
}

PerspectiveCamera* PerspectiveShadowMapVolume::GetLightCamera ()
{
	return _lightCamera;
}

std::vector<PipelineAttribute> PerspectiveShadowMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowMap;
	PipelineAttribute lightSpaceMatrix;
	PipelineAttribute shadowBias;

	shadowMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	shadowMap.name = "shadowMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";
	shadowBias.name = "shadowBias";

	shadowMap.value.x = _colorBuffer;

	glm::mat4 lightProjection = _lightCamera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCamera->GetRotation ()), _lightCamera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	lightSpaceMatrix.matrix = screenMatrix * lightProjection * lightView;
	shadowBias.value.x = _shadowBias;

	attributes.push_back (shadowMap);
	attributes.push_back (lightSpaceMatrix);
	attributes.push_back (shadowBias);

	return attributes;
}
