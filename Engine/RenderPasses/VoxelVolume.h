#ifndef VOXELSVOLUME_H
#define VOXELSVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <vector>

#include "Renderer/PipelineAttribute.h"

#include "Core/Math/glm/glm.hpp"

#define MIP_MAP_LEVELS 6

class VoxelVolume : public RenderVolumeI
{
protected:
	unsigned int _volumeTexture;
	unsigned int _volumeFbo;
	std::size_t _volumeSize;

	glm::vec3 _minVertex;
	glm::vec3 _maxVertex;

public:
	VoxelVolume ();
	virtual ~VoxelVolume ();

	virtual void Init (std::size_t size);

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual void BindForWriting (std::size_t mipmap);
	virtual std::vector<PipelineAttribute> GetCustomAttributes ();

	virtual void ClearVoxels();
	virtual void UpdateBoundingBox (const glm::vec3& minVertex, const glm::vec3& maxVertex);
protected:
	virtual void Clear ();
};

#endif