#ifndef SSDOSAMPLESVOLUME_H
#define SSDOSAMPLESVOLUME_H

#include "Renderer/RenderVolumeI.h"

#define SSDO_SAMPLES_NOT_INIT 391

struct SSDOSamples
{
	int samplesCount;
	int samplesCountPadding [3];
	float samples [800];
};

class ENGINE_API SSDOSamplesVolume : public RenderVolumeI
{
protected:
	SSDOSamples _samples;
	unsigned int _samplesUBO;

public:
	SSDOSamplesVolume ();

	bool Init (std::size_t samplesCount);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	std::size_t GetSamplesCount () const;

	void Clear ();
};

#endif
