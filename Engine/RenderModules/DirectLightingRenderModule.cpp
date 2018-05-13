#include "DirectLightingRenderModule.h"

#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/DirectionalLightShadowMapContainerRenderSubPass.h"
#include "RenderPasses/DeferredDirectionalLightContainerRenderSubPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/DeferredPointLightContainerRenderSubPass.h"
#include "RenderPasses/PointLightContainerRenderVolumeCollection.h"

void DirectLightingRenderModule::Init ()
{
	_renderPasses.push_back (new DeferredGeometryRenderPass ());	
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new DirectionalLightShadowMapContainerRenderSubPass ())
		.Attach (new DeferredDirectionalLightContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new PointLightContainerRenderVolumeCollection ())
		.Attach (new DeferredPointLightContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (new DeferredBlitRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
