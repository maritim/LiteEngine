#ifndef HGIINDIRECTSPECULARLIGHTRENDERPASS_H
#define HGIINDIRECTSPECULARLIGHTRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "RenderPasses/FramebufferRenderVolume.h"

class ENGINE_API HGIIndirectSpecularLightRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(HGIIndirectSpecularLightRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	void Init (const RenderSettings&);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings);
};

#endif