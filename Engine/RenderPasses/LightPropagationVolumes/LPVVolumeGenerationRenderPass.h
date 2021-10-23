#ifndef LPVVOLUMEGENERATIONRENDERPASS_H
#define LPVVOLUMEGENERATIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "LPVVolume.h"
#include "LPVGeometryVolume.h"
#include "LPVSampleCountVolume.h"

class ENGINE_API LPVVolumeGenerationRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(LPVVolumeGenerationRenderPass)

protected:
	LPVVolume* _lpvVolume;
	LPVGeometryVolume* _lpvGeometryVolume;
	LPVSampleCountVolume* _lpvSampleCountVolume;

public:
	LPVVolumeGenerationRenderPass ();
	~LPVVolumeGenerationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void UpdateLPVVolumeBoundingBox (const RenderScene* renderScene);
	void InitLPVVolume (const RenderSettings& settings);
	void UpdateLPVVolume (const RenderSettings& settings);
};

#endif