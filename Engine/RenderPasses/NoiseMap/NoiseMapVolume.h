#ifndef NOISEMAPVOLUME_H
#define NOISEMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/vec2.hpp>

#include "Renderer/PipelineAttribute.h"

#define NOISE_NOT_INIT 352

class ENGINE_API NoiseMapVolume : public RenderVolumeI
{
protected:
	unsigned int _noiseMap;
	unsigned int _noiseWidth;
	unsigned int _noiseHeight;

public:
	NoiseMapVolume ();
	virtual ~NoiseMapVolume ();

	virtual bool Init (std::size_t width, std::size_t height);

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	glm::ivec2 GetSize () const;
	unsigned int GetColorBuffer () const;

	virtual void Clear ();
};

#endif