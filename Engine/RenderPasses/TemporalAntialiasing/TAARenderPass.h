#ifndef TAARENDERPASS_H
#define TAARENDERPASS_H

#include "RenderPasses/PostProcess/PostProcessRenderPass.h"

#include "TAAMapVolume.h"

#define TEMPORAL_ANTI_ALIASING_MAP_VOLUME_NOT_INIT 354

class ENGINE_API TAARenderPass : public PostProcessRenderPass
{
	DECLARE_RENDER_PASS(TAARenderPass)

protected:
	PostProcessMapVolume* _taaMapVolume;

public:
	TAARenderPass ();
	~TAARenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void UpdateTAAMapVolume (const RenderSettings& settings);
	void InitTAAMapVolume (const RenderSettings& settings);
};

#endif