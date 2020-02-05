#ifndef VRTRENDERPASS_H
#define VRTRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class VRTRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(VRTRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	virtual ~VRTRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartRayTrace (RenderVolumeCollection* rvc);
	void VoxelRenderingRayTracePass (const Camera*, const RenderSettings&, RenderVolumeCollection*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif