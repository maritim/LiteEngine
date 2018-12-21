#ifndef SSAONOISEMAPVOLUME_H
#define SSAONOISEMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Renderer/PipelineAttribute.h"

class SSAONoiseMapVolume : public RenderVolumeI
{
protected:
	unsigned int _noiseMap;
	unsigned int _noiseWidth;
	unsigned int _noiseHeight;

public:
	SSAONoiseMapVolume ();
	virtual ~SSAONoiseMapVolume ();

	virtual bool Init (std::size_t width, std::size_t height);

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;
protected:
	virtual void Clear ();
};

#endif