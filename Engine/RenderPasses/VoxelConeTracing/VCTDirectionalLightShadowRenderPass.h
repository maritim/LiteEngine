#ifndef VCTDIRECTIONALLIGHTSHADOWRENDERPASS_H
#define VCTDIRECTIONALLIGHTSHADOWRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class VCTDirectionalLightShadowRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(VCTDirectionalLightShadowRenderPass)

protected:
	Resource<ShaderView> _shadowShaderView;

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderLightObject*) const;

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif