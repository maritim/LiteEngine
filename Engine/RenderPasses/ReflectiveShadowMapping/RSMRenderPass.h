#ifndef RSMRENDERPASS_H
#define RSMRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API RSMRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(RSMRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	void Init (const RenderSettings&);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
};

#endif