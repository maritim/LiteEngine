#ifndef HYBRIDRSMAMBIENTOCCLUSIONRENDERPASS_H
#define HYBRIDRSMAMBIENTOCCLUSIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "RenderPasses/FramebufferRenderVolume.h"

class ENGINE_API HybridRSMAmbientOcclusionRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(HybridRSMAmbientOcclusionRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	void Init (const RenderSettings&);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void Clear ();

	bool IsAvailable (const RenderLightObject*) const;

protected:
	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, const RenderLightObject* renderLightObject) const;
};

#endif