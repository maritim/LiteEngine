#ifndef SSDOSAMPLESVOLUME_H
#define SSDOSAMPLESVOLUME_H

#include "Renderer/RenderVolumeI.h"

#define SSDO_SAMPLES_NOT_INIT 391

struct SSDOSamples
{
	int samplesCount;
	int samplesCountPadding [3];
	float samples [2000];
};

class ENGINE_API SSDOSamplesVolume : public RenderVolumeI
{
protected:
	SSDOSamples _samples;
	unsigned int _samplesUBO;

	std::vector<PipelineAttribute> _attributes;

public:
	SSDOSamplesVolume (std::size_t samplesCount);
	~SSDOSamplesVolume ();

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	std::size_t GetSamplesCount () const;
};

#endif
