#ifndef IDLERENDERVOLUME_H
#define IDLERENDERVOLUME_H

#include "Renderer/RenderVolumeI.h"

class IdleRenderVolume : public RenderVolumeI
{
protected:
	std::vector<PipelineAttribute> _attributes;

public:
	virtual const std::vector<PipelineAttribute>& GetCustomAttributes () const;
};

#endif