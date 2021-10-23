#ifndef VPLCACHEVOLUME_H
#define VPLCACHEVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <map>
#include <glm/vec2.hpp>

class VPLCacheVolume : public RenderVolumeI
{
protected:
	std::size_t _samplesCount;
	unsigned int _samplesSSBO;

	std::vector<PipelineAttribute> _attributes;

public:
	VPLCacheVolume (std::size_t samplesCount);
	~VPLCacheVolume ();

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	std::size_t GetSamplesCount () const;
};

class VPLCachesVolume : public RenderVolumeI
{
protected:
	std::map<unsigned int, VPLCacheVolume*> _vplCacheVolumes;

public:
	~VPLCachesVolume ();

	const VPLCacheVolume* GetVPLCacheVolume (unsigned int vaoIndex) const;
	const VPLCacheVolume* InsertVPLCacheVolume (unsigned int vaoIndex, std::size_t samplesCount);
	const std::vector<PipelineAttribute>& GetCustomAttributes() const { static std::vector<PipelineAttribute> attributes; return attributes; }
};

#endif
