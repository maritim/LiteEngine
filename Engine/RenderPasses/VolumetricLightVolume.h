#ifndef VOLUMETRICLIGHTVOLUME_H
#define VOLUMETRICLIGHTVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Renderer/RenderLightObject.h"

class VolumetricLightVolume : public RenderVolumeI
{
protected:
	RenderLightObject* _renderLightObject;

	std::vector<PipelineAttribute> _attributes;

public:
	VolumetricLightVolume ();

	void SetRenderLightObject (RenderLightObject* renderLightObject);
	RenderLightObject* GetRenderLightObject () const;

	virtual const std::vector<PipelineAttribute>& GetCustomAttributes () const;
};

#endif