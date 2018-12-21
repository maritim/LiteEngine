#ifndef SSAOSAMPLESVOLUME_H
#define SSAOSAMPLESVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/glm.hpp>

#define SSAO_SAMPLES_NOT_INIT 351

class SSAOSamplesVolume : public RenderVolumeI
{
protected:
	std::vector<glm::vec3> _samples;

public:
	SSAOSamplesVolume ();

	bool Init (std::size_t samplesCount);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif
