#ifndef VOXELSVOLUME_H
#define VOXELSVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/glm.hpp>
#include <vector>

#include "Renderer/PipelineAttribute.h"

#define VOXEL_TEXTURE_NOT_INIT 360

class VoxelVolume : public RenderVolumeI
{
protected:
	unsigned int _volumeTexture;
	unsigned int _volumeFbo;
	std::size_t _volumeSize;
	std::size_t _volumeMipmapLevels;

	glm::vec3 _minVertex;
	glm::vec3 _maxVertex;

public:
	VoxelVolume ();
	virtual ~VoxelVolume ();

	virtual bool Init (std::size_t size);

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual void BindForWriting (std::size_t mipmap, bool permit = false);
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	virtual void ClearVoxels();
	virtual void UpdateBoundingBox (const glm::vec3& minVertex, const glm::vec3& maxVertex);

	void SetVolumeMipmapLevels (std::size_t volumeMipmapLevels);

	std::size_t GetVolumeSize () const;
	std::size_t GetVolumeMipmapLevels () const;

	virtual void Clear ();
};

#endif