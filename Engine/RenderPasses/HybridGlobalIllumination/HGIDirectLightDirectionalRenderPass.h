#ifndef HGIDIRECTLIGHTDIRECTIONALRENDERPASS_H
#define HGIDIRECTLIGHTDIRECTIONALRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "RenderPasses/FramebufferRenderVolume.h"

class ENGINE_API HGIDirectLightDirectionalRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(HGIDirectLightDirectionalRenderPass)

protected:
	Resource<ShaderView> _shadowShaderView;

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif