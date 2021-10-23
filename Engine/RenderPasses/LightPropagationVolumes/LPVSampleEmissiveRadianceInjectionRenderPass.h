#ifndef LPVSAMPLEEMISSIVERADIANCEINJECTIONRENDERPASS_H
#define LPVSAMPLEEMISSIVERADIANCEINJECTIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class LPVSampleEmissiveRadianceInjectionRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(LPVSampleEmissiveRadianceInjectionRenderPass)

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
	void GeometryVoxelizationPass (const RenderScene* renderScene, const RenderSettings& settings,
		RenderVolumeCollection* rvc);
	void EndVoxelization ();

	void LockShader (int sceneLayers);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings);

	//virtual void UpdateVPLCacheVolume (const RenderSettings& settings);
};

#endif