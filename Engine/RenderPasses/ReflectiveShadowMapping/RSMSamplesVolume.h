#ifndef RSMSAMPLESVOLUME_H
#define RSMSAMPLESVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/vec2.hpp>

#define REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT 351

struct RSMSamples
{
	int samplesCount;
	int samplesCountPadding [3];
	float samples [2000];
};

class RSMSamplesVolume : public RenderVolumeI
{
protected:
	RSMSamples _samples;
	unsigned int _samplesUBO;

	std::vector<PipelineAttribute> _attributes;

public:
	RSMSamplesVolume (std::size_t samplesCount);
	~RSMSamplesVolume ();

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	std::size_t GetSize () const;
};

#endif
