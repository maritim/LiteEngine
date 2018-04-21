#include "ReflectiveShadowMapRenderModule.h"

#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapDirectionalLightAccumulationContainerRenderSubPass.h"
#include "RenderPasses/DirectionalLightShadowMapContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapSamplesGenerationContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapDirectionalLightContainerRenderSubPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

void ReflectiveShadowMapRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new ReflectiveShadowMapDirectionalLightAccumulationContainerRenderSubPass ())
		.Attach (new DirectionalLightShadowMapContainerRenderSubPass ())
		.Attach (new ReflectiveShadowMapSamplesGenerationContainerRenderSubPass ())
		.Attach (new ReflectiveShadowMapDirectionalLightContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (new DeferredBlitRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
