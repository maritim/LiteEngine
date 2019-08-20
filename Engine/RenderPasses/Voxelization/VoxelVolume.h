#ifndef VOXELSVOLUME_H
#define VOXELSVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/glm.hpp>
#include <vector>

#include "Renderer/PipelineAttribute.h"

#define MIP_MAP_LEVELS 6

#define VOXEL_TEXTURE_NOT_INIT 360

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

	virtual bool Init (std::size_t size);

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual void BindForWriting (std::size_t mipmap);
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	virtual void ClearVoxels();
	virtual void UpdateBoundingBox (const glm::vec3& minVertex, const glm::vec3& maxVertex);

	std::size_t GetVolumeSize () const;

	virtual void Clear ();
};

#endif