#ifndef DIRECTIONALLIGHTSOURCERENDERPASS_H
#define DIRECTIONALLIGHTSOURCERENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API DirectionalLightSourceRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DirectionalLightSourceRenderPass)

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