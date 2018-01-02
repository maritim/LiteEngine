#include "VoxelConeTraceDirectionalLightRenderPass.h"

#include "DeferredDirectionalLightShadowMapRenderPass.h"
#include "DeferredDirectionalVolumetricLightRenderPass.h"

void VoxelConeTraceDirectionalLightRenderPass::InitContainer ()
{
	_renderSubPasses.push_back (new DeferredDirectionalLightShadowMapRenderPass ());
	_renderSubPasses.push_back (new DeferredDirectionalVolumetricLightRenderPass ());
}
