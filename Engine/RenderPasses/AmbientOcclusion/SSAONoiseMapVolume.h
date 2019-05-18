#ifndef SSAONOISEMAPVOLUME_H
#define SSAONOISEMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/vec2.hpp>

#include "Renderer/PipelineAttribute.h"

class ENGINE_API SSAONoiseMapVolume : public RenderVolumeI
{
protected:
	unsigned int _noiseMap;
	unsigned int _noiseWidth;
	unsigned int _noiseHeight;

public:
	SSAONoiseMapVolume ();
	virtual ~SSAONoiseMapVolume ();

	virtual bool Init (std::size_t width, std::size_t height);

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	glm::ivec2 GetSize () const;

	virtual void Clear ();
};

#endif