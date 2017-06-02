#ifndef VOXELSVOLUME_H
#define VOXELSVOLUME_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "Renderer/PipelineAttribute.h"

#include "Core/Math/glm/glm.hpp"

class VoxelVolume : public Object
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

	virtual void ClearVoxels();
	virtual void UpdateBoundingBox (const glm::vec3& minVertex, const glm::vec3& maxVertex);
	virtual void GenerateMipmaps ();

	virtual void Init (std::size_t size);
	virtual void StartVoxelizationPass ();
	virtual void BindForVoxelizationPass ();
	virtual void EndVoxelizationPass ();

	virtual void StartRayTracePass ();
	virtual void BindForRayTracePass ();

	virtual void StartConeTracePass ();
	virtual void BindForConeTraceLightPass ();

	virtual std::vector<PipelineAttribute> GetVoxelConeTracePipelineAttributes ();
protected:
	virtual void Clear ();

	virtual void UpdateVoxelRayTracePipelineAttributes ();
	virtual void UpdateVoxelizationPipelineAttributes ();
};

#endif