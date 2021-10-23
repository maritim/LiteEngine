#ifndef LPVCACHEEMISSIVERADIANCEINJECTIONRENDERPASS_H
#define LPVCACHEEMISSIVERADIANCEINJECTIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class LPVCacheEmissiveRadianceInjectionRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(LPVCacheEmissiveRadianceInjectionRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	void StartVoxelization ();
	void GeometryVoxelizationPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void EndVoxelization ();

	void LockShader (int sceneLayers);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings);
};

#endif