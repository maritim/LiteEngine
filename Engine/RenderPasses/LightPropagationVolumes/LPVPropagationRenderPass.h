#ifndef LPVPROPAGATIONRENDERPASS_H
#define LPVPROPAGATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "LPVVolume.h"

class ENGINE_API LPVPropagationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(LPVPropagationRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	LPVVolume* _lpvVolume;

public:
	LPVPropagationRenderPass ();
	~LPVPropagationRenderPass ();

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

	void InitLPVVolume (const RenderSettings& settings);
	void UpdateLPVVolume (const RenderSettings& settings);
};

#endif