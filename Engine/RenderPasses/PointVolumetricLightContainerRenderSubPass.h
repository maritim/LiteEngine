#ifndef POINTVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H
#define POINTVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

class PointVolumetricLightContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
public:
	~PointVolumetricLightContainerRenderSubPass ();

	RenderVolumeCollection* Execute (const Scene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const VolumetricLight*) const;

	void StartPointLightPass (RenderVolumeCollection*);
	void PointLightPass (const Scene*, const Camera*, RenderVolumeCollection*);
	void EndPointLightPass ();

	void PointLightStencilPass (const Scene*, const Camera*, VolumetricLight*, RenderVolumeCollection*);
	void PointLightDrawPass (const Scene*, const Camera*, VolumetricLight*, RenderVolumeCollection*);

	virtual void LockShader (const VolumetricLight*) = 0;
};

#endif