#include "VoxelVolume.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "Debug/Logger/Logger.h"

VoxelVolume::VoxelVolume (const Resource<Framebuffer>& framebuffer, std::size_t mipmapLevels) :
	FramebufferRenderVolume (framebuffer)
{
	/*
	 * Create attributes
	*/

	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSizeAttribute;
	PipelineAttribute volumeMipmapLevels;

	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSizeAttribute.type = PipelineAttribute::AttrType::ATTR_3I;
	volumeMipmapLevels.type = PipelineAttribute::AttrType::ATTR_1I;

	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSizeAttribute.name = "volumeSize";
	volumeMipmapLevels.name = "volumeMipmapLevels";

	volumeSizeAttribute.value = glm::vec3 ((float) _framebuffer->GetTexture (0)->GetSize ().width);
	volumeMipmapLevels.value.x = mipmapLevels;

	_attributes.push_back (minVertex);
	_attributes.push_back (maxVertex);
	_attributes.push_back (volumeSizeAttribute);
	_attributes.push_back (volumeMipmapLevels);
}

void VoxelVolume::UpdateBoundingBox(const glm::vec3& minVertex, const glm::vec3& maxVertex)
{
	float sizeX = maxVertex.x - minVertex.x;
	float sizeY = maxVertex.y - minVertex.y;
	float sizeZ = maxVertex.z - minVertex.z;

	float difX, difY, difZ;
	difX = difY = difZ = 0;

	if (sizeX >= sizeY && sizeX >= sizeZ) {
		difY = sizeX - sizeY;
		difZ = sizeX - sizeZ;
	}
	else if (sizeY >= sizeZ) {
		difX = sizeY - sizeX;
		difZ = sizeZ - sizeX;
	}
	else {
		difX = sizeZ - sizeX;
		difY = sizeZ - sizeY;
	}

	/*
	 * Update attributes
	*/

	_attributes [2].value = minVertex - glm::vec3(difX / 2.0f, difY / 2.0f, difZ / 2.0f);
	_attributes [3].value = maxVertex + glm::vec3(difX / 2.0f, difY / 2.0f, difZ / 2.0f);
}

const glm::vec3& VoxelVolume::GetMinVertex () const
{
	return _attributes [2].value;
}

const glm::vec3& VoxelVolume::GetMaxVertex () const
{
	return _attributes [3].value;
}

std::size_t VoxelVolume::GetMipmapLevels () const
{
	return (std::size_t) _attributes [5].value.x;
}
