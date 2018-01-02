#include "DeferredDirectionalLightRenderPass.h"

#include "DeferredDirectionalLightShadowMapRenderPass.h"
#include "DeferredDirectionalVolumetricLightRenderPass.h"

void DeferredDirectionalLightRenderPass::InitContainer ()
{
	_renderSubPasses.push_back (new DeferredDirectionalLightShadowMapRenderPass ());
	_renderSubPasses.push_back (new DeferredDirectionalVolumetricLightRenderPass ());
}