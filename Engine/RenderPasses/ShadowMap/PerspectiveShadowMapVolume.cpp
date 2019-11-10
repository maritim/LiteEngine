#include "PerspectiveShadowMapVolume.h"

PerspectiveShadowMapVolume::PerspectiveShadowMapVolume () :
	_lightCamera (nullptr)
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

PerspectiveCamera* PerspectiveShadowMapVolume::GetLightCamera ()
{
	return _lightCamera;
}

std::vector<PipelineAttribute> PerspectiveShadowMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowMap;
	PipelineAttribute lightSpaceMatrix;

	shadowMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	shadowMap.name = "shadowMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";

	shadowMap.value.x = _colorBuffer;

	glm::mat4 lightProjection = _lightCamera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCamera->GetRotation ()), _lightCamera->GetPosition () * -1.0f);

	lightSpaceMatrix.matrix = lightProjection * lightView;

	attributes.push_back (shadowMap);
	attributes.push_back (lightSpaceMatrix);

	return attributes;
}
