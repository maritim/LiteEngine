#ifndef VOLUMETRICLIGHTVOLUME_H
#define VOLUMETRICLIGHTVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Renderer/RenderLightObject.h"

class VolumetricLightVolume : public RenderVolumeI
{
protected:
	RenderLightObject* _renderLightObject;

public:
	VolumetricLightVolume ();

	void SetRenderLightObject (RenderLightObject* renderLightObject);
	RenderLightObject* GetRenderLightObject () const;

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	void Clear ();
};

#endif