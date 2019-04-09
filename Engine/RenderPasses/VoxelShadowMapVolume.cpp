#include "VoxelShadowMapVolume.h"

#include <glm/gtc/matrix_transform.hpp>

void VoxelShadowMapVolume::SetLightCamera (Camera* camera)
{
	_camera = camera;
}

Camera* VoxelShadowMapVolume::GetLightCamera ()
{
	return _camera;
}

void VoxelShadowMapVolume::BindForReading ()
{
	/*
	 * Bind for reading first shadow map cascade
	*/

	CascadedShadowMapDirectionalLightVolume::BindForReading ();
}

void VoxelShadowMapVolume::BindForWriting ()
{
	/*
	 * Bind for writing on first cascade shadow map
	*/

	BindForShadowMapCatch (0);
}

std::vector<PipelineAttribute> VoxelShadowMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowMap;
	PipelineAttribute lightSpaceMatrix;

	shadowMap.type = PipelineAttribute::AttrType::ATTR_1I;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	shadowMap.name = "shadowMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";

	shadowMap.value.x = 4;

	glm::mat4 lightProjection = _camera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (_camera->GetRotation ()), _camera->GetPosition () * -1.0f);

	lightSpaceMatrix.matrix = lightProjection * lightView;

	attributes.push_back (shadowMap);
	attributes.push_back (lightSpaceMatrix);

	return attributes;
}
