#ifndef DIRECTIONALLIGHTEXPONENTIALSHADOWMAPRENDERPASS_H
#define DIRECTIONALLIGHTEXPONENTIALSHADOWMAPRENDERPASS_H

#include "RenderPasses/ShadowMap/DirectionalLightShadowMapRenderPass.h"

class DirectionalLightExponentialShadowMapRenderPass : public DirectionalLightShadowMapRenderPass
{
protected:
	int _exponential;

public:
	DirectionalLightExponentialShadowMapRenderPass ();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
	void Init ();
#pragma clang diagnostic pop
	
protected:
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void InitShadowMapVolume (const RenderLightObject* renderLightObject);
};

#endif