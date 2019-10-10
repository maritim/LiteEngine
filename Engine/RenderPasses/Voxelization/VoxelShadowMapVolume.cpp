#include "VoxelShadowMapVolume.h"

#include <glm/gtc/matrix_transform.hpp>

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

	PipelineAttribute cascadesCount;
	PipelineAttribute shadowMap;
	PipelineAttribute lightSpaceMatrix;

	cascadesCount.type = PipelineAttribute::AttrType::ATTR_1I;
	shadowMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	cascadesCount.name = "cascadesCount";
	shadowMap.name = "shadowMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";

	cascadesCount.value.x = _cascadedLevels;
	shadowMap.value.x = _shadowMaps [0]->GetColorTextureID ();

	Camera* lightCamera = _lightCameras [0];
	glm::mat4 lightProjection = lightCamera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightCamera->GetRotation ()), lightCamera->GetPosition () * -1.0f);

	lightSpaceMatrix.matrix = lightProjection * lightView;

	attributes.push_back (shadowMap);
	attributes.push_back (lightSpaceMatrix);

	return attributes;
}
