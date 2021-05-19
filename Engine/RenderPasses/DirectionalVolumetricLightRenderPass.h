#ifndef DIRECTIONALVOLUMETRICLIGHTRENDERPASS_H
#define DIRECTIONALVOLUMETRICLIGHTRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

class ENGINE_API DirectionalVolumetricLightRenderPass : public VolumetricLightRenderPassI
{
public:
	~DirectionalVolumetricLightRenderPass ();

	RenderVolumeCollection* Execute (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const RenderLightObject*) const;

	virtual void StartDirectionalLightPass (RenderVolumeCollection*);
	void DirectionalLightPass (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection*);
	void EndDirectionalLightPass ();

	virtual void LockShader (const RenderLightObject*) = 0;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera*, const RenderLightObject*, RenderVolumeCollection*) const;

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings&) const = 0;
};

#endif