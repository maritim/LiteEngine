#ifndef LPVSAMPLEWEIGHTRENDERPASS_H
#define LPVSAMPLEWEIGHTRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API LPVSampleWeightRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(LPVSampleWeightRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void StartPostProcessPass ();
	void PostProcessPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void EndPostProcessPass ();
};

#endif