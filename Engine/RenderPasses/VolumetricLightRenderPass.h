#ifndef VOLUMETRICLIGHTRENDERPASS_H
#define VOLUMETRICLIGHTRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

class ENGINE_API VolumetricLightRenderPass : public VolumetricLightRenderPassI
{
protected:
	std::string _stencilShaderName;

public:
	VolumetricLightRenderPass ();
	~VolumetricLightRenderPass ();

	void Init (const RenderSettings&);

	RenderVolumeCollection* Execute (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void StartPointLightPass (RenderVolumeCollection*);
	void PointLightPass (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection*);
	void EndPointLightPass ();

	void PointLightStencilPass (const RenderScene*, const Camera*, const RenderSettings&, RenderLightObject*, RenderVolumeCollection*);
	void PointLightDrawPass (const RenderScene*, const Camera*, const RenderSettings&, RenderLightObject*, RenderVolumeCollection*);

	virtual void LockShader (const RenderLightObject*) = 0;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera*, const RenderLightObject*, RenderVolumeCollection*) const;

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const RenderLightObject*) const = 0;
};

#endif