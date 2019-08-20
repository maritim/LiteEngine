#ifndef POINTVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H
#define POINTVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

class ENGINE_API PointVolumetricLightContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
public:
	~PointVolumetricLightContainerRenderSubPass ();

	RenderVolumeCollection* Execute (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void StartPointLightPass (RenderVolumeCollection*);
	void PointLightPass (const RenderScene*, const Camera*, RenderVolumeCollection*);
	void EndPointLightPass ();

	void PointLightStencilPass (const RenderScene*, const Camera*, RenderLightObject*, RenderVolumeCollection*);
	void PointLightDrawPass (const RenderScene*, const Camera*, RenderLightObject*, RenderVolumeCollection*);

	virtual void LockShader (const RenderLightObject*) = 0;
};

#endif