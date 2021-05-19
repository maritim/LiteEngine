#ifndef FRAMEBUFFERMIPMAPSGENERATIONRENDERPASS_H
#define FRAMEBUFFERMIPMAPSGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "RenderPasses/FramebufferRenderVolume.h"

class FramebufferMipmapsGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(FramebufferMipmapsGenerationRenderPass)

protected:
	std::string _volumeName;

public:
	FramebufferMipmapsGenerationRenderPass (const std::string& volumeName = std::string ());

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void GenerateMipmaps (const RenderVolumeCollection* rvc) const;
};

#endif