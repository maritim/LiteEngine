#ifndef VOLUMETRICLIGHTVOLUME_H
#define VOLUMETRICLIGHTVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Lighting/VolumetricLight.h"

class VolumetricLightVolume : public RenderVolumeI
{
protected:
	VolumetricLight* _volumetricLight;

public:
	VolumetricLightVolume ();

	void SetVolumetricLight (VolumetricLight* volumetricLight);
	VolumetricLight* GetVolumetricLight () const;

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	void Clear ();
};

#endif