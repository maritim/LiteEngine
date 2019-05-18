#ifndef DIRECTIONALVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H
#define DIRECTIONALVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

class ENGINE_API DirectionalVolumetricLightContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
public:
	~DirectionalVolumetricLightContainerRenderSubPass ();

	RenderVolumeCollection* Execute (const Scene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const VolumetricLight*) const;

	void StartDirectionalLightPass (RenderVolumeCollection*);
	void DirectionalLightPass (const Scene*, const Camera*, const RenderSettings&, RenderVolumeCollection*);
	void EndDirectionalLightPass ();

	virtual void LockShader (const VolumetricLight*) = 0;

	virtual std::vector<PipelineAttribute> GetCustomAttributes () const = 0;
};

#endif