#ifndef TRSMINDIRECTLIGHTRENDERPASS_H
#define TRSMINDIRECTLIGHTRENDERPASS_H

#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectLightRenderPass.h"

class ENGINE_API TRSMIndirectLightRenderPass : public RSMIndirectLightRenderPass
{
	DECLARE_RENDER_PASS(TRSMIndirectLightRenderPass)

protected:
	PostProcessMapVolume* _lastPostProcessMapVolume;

public:
	TRSMIndirectLightRenderPass ();
	~TRSMIndirectLightRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	void UpdateTRSMIndirectMapVolume (const RenderSettings& settings);
	void InitTRSMIndirectMapVolume (const RenderSettings& settings);
};

#endif