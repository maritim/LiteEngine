#include "ReflectiveShadowMapRenderModule.h"

#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapDirectionalLightRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

void ReflectiveShadowMapRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (new ReflectiveShadowMapDirectionalLightRenderPass ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (new DeferredBlitRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
