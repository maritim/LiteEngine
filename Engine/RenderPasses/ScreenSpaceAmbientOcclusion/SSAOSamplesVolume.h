#ifndef SSAOSAMPLESVOLUME_H
#define SSAOSAMPLESVOLUME_H

#include "Renderer/RenderVolumeI.h"

#define SSAO_SAMPLES_NOT_INIT 351

struct SSAOSamples
{
	int samplesCount;
	int samplesCountPadding [3];
	float samples [800];
};

class ENGINE_API SSAOSamplesVolume : public RenderVolumeI
{
protected:
	SSAOSamples _samples;
	unsigned int _samplesUBO;

	std::vector<PipelineAttribute> _attributes;

public:
	SSAOSamplesVolume (std::size_t samplesCount);
	~SSAOSamplesVolume ();

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	std::size_t GetSamplesCount () const;
};

#endif
