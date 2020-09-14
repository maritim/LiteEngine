#ifndef VOXELSVOLUME_H
#define VOXELSVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

#include <glm/glm.hpp>
#include <vector>

#include "Renderer/PipelineAttribute.h"

#define VOXEL_TEXTURE_NOT_INIT 360

class VoxelVolume : public FramebufferRenderVolume
{
public:
	VoxelVolume (const Resource<Framebuffer>& framebuffer);

	virtual void UpdateBoundingBox (const glm::vec3& minVertex, const glm::vec3& maxVertex);

	const glm::vec3& GetMinVertex () const;
	const glm::vec3& GetMaxVertex () const;
};

#endif