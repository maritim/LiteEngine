#ifndef POSTPROCESSRENDERPASS_H
#define POSTPROCESSRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <vector>
#include <glm/vec2.hpp>

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Renderer/PipelineAttribute.h"

class ENGINE_API PostProcessRenderPass : public ContainerRenderSubPassI
{
protected:
	Resource<ShaderView> _shaderView;
	FramebufferRenderVolume* _postProcessMapVolume;

public:
	PostProcessRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	virtual void StartPostProcessPass ();
	virtual void PostProcessPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual void EndPostProcessPass ();

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual void UpdatePostProcessSettings (const RenderSettings& settings);

	virtual std::string GetPostProcessFragmentShaderPath () const = 0;
	virtual std::string GetPostProcessVolumeName () const = 0;
	virtual glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const = 0;
	virtual FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const = 0;
};

#endif