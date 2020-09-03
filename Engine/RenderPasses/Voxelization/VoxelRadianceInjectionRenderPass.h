#ifndef VOXELRADIANCEINJECTIONRENDERPASS_H
#define VOXELRADIANCEINJECTIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "VoxelVolume.h"

class ENGINE_API VoxelRadianceInjectionRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(VoxelRadianceInjectionRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void StartPostProcessPass (RenderVolumeCollection* rvc);
	void PostProcessPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderLightObject* renderLightObject,
		RenderVolumeCollection* rvc);
	void EndPostProcessPass ();

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings,
		const RenderLightObject* renderLightObject) const;
};

#endif