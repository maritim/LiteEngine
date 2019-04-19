#ifndef DIRECTIONALLIGHTEXPONENTIALSHADOWMAPCONTAINERRENDERSUBPASS_H
#define DIRECTIONALLIGHTEXPONENTIALSHADOWMAPCONTAINERRENDERSUBPASS_H

#include "RenderPasses/ShadowMap/DirectionalLightShadowMapContainerRenderSubPass.h"

class DirectionalLightExponentialShadowMapContainerRenderSubPass : public DirectionalLightShadowMapContainerRenderSubPass
{
protected:
	int _exponential;

public:
	DirectionalLightExponentialShadowMapContainerRenderSubPass ();

	void Init ();

	void Notify (Object* sender, const SettingsObserverArgs& args);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void InitSettings ();
	void ClearSettings ();

	void InitShadowMapVolume ();
};

#endif