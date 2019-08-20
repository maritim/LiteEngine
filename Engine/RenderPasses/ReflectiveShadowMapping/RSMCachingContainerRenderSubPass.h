#ifndef RSMCACHINGCONTAINERRENDERSUBPASS_H
#define RSMCACHINGCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <string>

#include "RSMCacheVolume.h"

class RSMCachingContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	std::string _shaderName;
	RSMCacheVolume* _rsmCacheVolume;

public:
	RSMCachingContainerRenderSubPass ();
	virtual ~RSMCachingContainerRenderSubPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	virtual void StartPostProcessPass ();
	virtual void PostProcessPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual void EndPostProcessPass ();

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual void ReinitRSMCacheVolume (const RenderSettings& settings);

	virtual void UpdateRSMCachingSettings (const RenderSettings& settings);
};

#endif