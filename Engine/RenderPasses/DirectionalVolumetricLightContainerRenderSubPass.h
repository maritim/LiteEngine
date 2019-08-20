#ifndef DIRECTIONALVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H
#define DIRECTIONALVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

class ENGINE_API DirectionalVolumetricLightContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
public:
	~DirectionalVolumetricLightContainerRenderSubPass ();

	RenderVolumeCollection* Execute (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void StartDirectionalLightPass (RenderVolumeCollection*);
	void DirectionalLightPass (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection*);
	void EndDirectionalLightPass ();

	virtual void LockShader (const RenderLightObject*) = 0;

	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection*) const;

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings&) const = 0;
};

#endif