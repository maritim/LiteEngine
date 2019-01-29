#ifndef IDLERENDERVOLUME_H
#define IDLERENDERVOLUME_H

#include "Renderer/RenderVolumeI.h"

class IdleRenderVolume : public RenderVolumeI
{
public:
	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	void Clear ();
};

#endif