#ifndef VOLUMETRICLIGHTINGDIRECTIONALRENDERPASS_H
#define VOLUMETRICLIGHTINGDIRECTIONALRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API VolumetricLightingDirectionalRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(VolumetricLightingDirectionalRenderPass)

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