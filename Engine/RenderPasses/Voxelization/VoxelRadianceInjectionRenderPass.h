#ifndef VOXELRADIANCEINJECTIONRENDERPASS_H
#define VOXELRADIANCEINJECTIONRENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelRadianceInjectionRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(VoxelRadianceInjectionRenderPass)

protected:
	bool _firstTime;

public:
	VoxelRadianceInjectionRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartRadianceInjectionPass ();
	void RadianceInjectPass (const RenderSettings&, RenderVolumeCollection*);
	void EndRadianceInjectionPass ();

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings&) const;
};

#endif