#ifndef POINTVOLUMETRICLIGHTRENDERPASS_H
#define POINTVOLUMETRICLIGHTRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

class ENGINE_API PointVolumetricLightRenderPass : public VolumetricLightRenderPassI
{
public:
	~PointVolumetricLightRenderPass ();

	RenderVolumeCollection* Execute (const RenderScene*, const Camera*, const RenderSettings&, RenderVolumeCollection* );
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void StartPointLightPass (RenderVolumeCollection*);
	void PointLightPass (const RenderScene*, const Camera*, RenderVolumeCollection*);
	void EndPointLightPass ();

	void PointLightStencilPass (const RenderScene*, const Camera*, RenderLightObject*, RenderVolumeCollection*);
	void PointLightDrawPass (const RenderScene*, const Camera*, RenderLightObject*, RenderVolumeCollection*);

	virtual void LockShader (const RenderLightObject*) = 0;

	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection*) const;
};

#endif