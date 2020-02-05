#ifndef EXPONENTIALSHADOWMAPBLURRENDERPASS_H
#define EXPONENTIALSHADOWMAPBLURRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"
#include "Core/Observer/ObserverI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "ExponentialShadowMapBlurVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class ExponentialShadowMapBlurRenderPass : public VolumetricLightRenderPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	Resource<ShaderView> _horizontalShaderViewer;
	Resource<ShaderView> _verticalShaderViewer;
	ExponentialShadowMapBlurVolume** _framebuffers;

	glm::ivec2 _resolution;
	std::size_t _cascades;

public:
	ExponentialShadowMapBlurRenderPass ();
	~ExponentialShadowMapBlurRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void Blur (ExponentialShadowMapBlurVolume* fb1, ExponentialShadowMapBlurVolume* fb2);

	void InitSettings ();
	void ClearSettings ();

	void InitShadowMapBlurVolumes ();
};

#endif