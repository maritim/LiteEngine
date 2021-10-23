#ifndef LPVCACHEEMISSIVERADIANCERENDERPASS_H
#define LPVCACHEEMISSIVERADIANCERENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "VPLCacheVolume.h"

class LPVCacheEmissiveRadianceRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(LPVCacheEmissiveRadianceRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	VPLCachesVolume* _vplCachesVolume;

public:
	LPVCacheEmissiveRadianceRenderPass ();
	~LPVCacheEmissiveRadianceRenderPass ();

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

	void LockShader ();

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings);
};

#endif