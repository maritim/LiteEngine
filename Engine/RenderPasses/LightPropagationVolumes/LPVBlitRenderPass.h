#ifndef LPVBLITRENDERPASS_H
#define LPVBLITRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "LPVPropagationVolume.h"

class ENGINE_API LPVBlitRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(LPVBlitRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	LPVPropagationVolume* _lpvPropagationVolume;

public:
	LPVBlitRenderPass ();
	~LPVBlitRenderPass ();

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