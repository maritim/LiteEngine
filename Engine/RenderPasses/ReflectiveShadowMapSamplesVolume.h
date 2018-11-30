#ifndef REFLECTIVESHADOWMAPSAMPLESVOLUME_H
#define REFLECTIVESHADOWMAPSAMPLESVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/glm.hpp>

#define REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT 351

class ReflectiveShadowMapSamplesVolume : public RenderVolumeI
{
protected:
	std::vector<glm::vec2> _samples;
public:
	ReflectiveShadowMapSamplesVolume ();

	bool Init (std::size_t samplesCount);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif
