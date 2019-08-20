#ifndef POSTPROCESSCONTAINERRENDERSUBPASS_H
#define POSTPROCESSCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <vector>
#include <string>
#include <glm/vec2.hpp>

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

#include "Renderer/PipelineAttribute.h"

class ENGINE_API PostProcessContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	PostProcessMapVolume* _postProcessMapVolume;

public:
	PostProcessContainerRenderSubPass ();
	virtual ~PostProcessContainerRenderSubPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	virtual void StartPostProcessPass ();
	virtual void PostProcessPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual void EndPostProcessPass ();

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual void ReinitPostProcessVolume (const RenderSettings& settings);

	virtual void UpdatePostProcessSettings (const RenderSettings& settings);

	virtual std::string GetPostProcessFragmentShaderPath () const = 0;
	virtual std::string GetPostProcessVolumeName () const = 0;
	virtual glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const = 0;
	virtual PostProcessMapVolume* CreatePostProcessVolume () const = 0;
};

#endif