#ifndef AMBIENTLIGHTVOLUME_H
#define AMBIENTLIGHTVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Utils/Color/Color.h"

class AmbientLightVolume : public RenderVolumeI
{
protected:
	std::vector<PipelineAttribute> _attributes;

public:
	void SetAmbientLight (float, const Color&, bool);

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;
};

#endif
