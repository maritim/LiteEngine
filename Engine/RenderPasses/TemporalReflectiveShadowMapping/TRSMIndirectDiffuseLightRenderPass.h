#ifndef TRSMINDIRECTDIFFUSELIGHTRENDERPASS_H
#define TRSMINDIRECTDIFFUSELIGHTRENDERPASS_H

#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectDiffuseLightRenderPass.h"

class ENGINE_API TRSMIndirectDiffuseLightRenderPass : public RSMIndirectDiffuseLightRenderPass
{
	DECLARE_RENDER_PASS(TRSMIndirectDiffuseLightRenderPass)

protected:
	PostProcessMapVolume* _lastPostProcessMapVolume;

public:
	TRSMIndirectDiffuseLightRenderPass();
	~TRSMIndirectDiffuseLightRenderPass();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	void UpdateTRSMIndirectDiffuseMapVolume (const RenderSettings& settings);
	void InitTRSMIndirectDiffuseMapVolume (const RenderSettings& settings);
};

#endif