#include "ReflectiveShadowMapDirectionalLightRenderPass.h"

#include "ReflectiveShadowMapSamplesGenerationPass.h"
#include "DeferredDirectionalLightShadowMapRenderPass.h"
#include "ReflectiveDirectionalShadowMapAccumulationPass.h"
#include "ReflectiveShadowMapDirectionalVolumetricLightRenderPass.h"

void ReflectiveShadowMapDirectionalLightRenderPass::InitContainer ()
{
	_renderSubPasses.push_back (new ReflectiveShadowMapSamplesGenerationPass ());
	_renderSubPasses.push_back (new DeferredDirectionalLightShadowMapRenderPass ());
	_renderSubPasses.push_back (new ReflectiveDirectionalShadowMapAccumulationPass ());
	_renderSubPasses.push_back (new ReflectiveShadowMapDirectionalVolumetricLightRenderPass ());
}