#ifndef FRAMEBUFFERGENERATIONRENDERPASS_H
#define FRAMEBUFFERGENERATIONRENDERPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "FramebufferRenderVolume.h"

class ENGINE_API FramebufferGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(FramebufferGenerationRenderPass)

protected:
	FramebufferRenderVolume* _volume;
	std::string _volumeName;

public:
	FramebufferGenerationRenderPass (const std::string& _name);

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateVolume (const RenderSettings& settings);

	void InitVolume (const RenderSettings& settings);	
};

#endif