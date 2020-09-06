#ifndef TEXTURELUTRENDERPASS_H
#define TEXTURELUTRENDERPASS_H

#include "RenderPasses/PostProcess/PostProcessRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/TextureLUTView.h"

class ENGINE_API TextureLUTRenderPass : public PostProcessRenderPass
{
	DECLARE_RENDER_PASS(TextureLUTRenderPass)

protected:
	std::string _lutTexturePath;
	Resource<TextureView> _lutTextureView;

public:
	void Init (const RenderSettings& settings);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void UpdateTextureLUTView (const RenderSettings& settings);
	void InitTextureLUTView (const RenderSettings& settings);
};

#endif