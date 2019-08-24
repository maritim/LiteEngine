#ifndef DIRECTIONALLIGHTEXPONENTIALSHADOWMAPRENDERPASS_H
#define DIRECTIONALLIGHTEXPONENTIALSHADOWMAPRENDERPASS_H

#include "RenderPasses/ShadowMap/DirectionalLightShadowMapRenderPass.h"

class DirectionalLightExponentialShadowMapRenderPass : public DirectionalLightShadowMapRenderPass
{
protected:
	int _exponential;

public:
	DirectionalLightExponentialShadowMapRenderPass ();

	void Init ();

	void Notify (Object* sender, const SettingsObserverArgs& args);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void InitSettings ();
	void ClearSettings ();

	void InitShadowMapVolume ();
};

#endif