#include "ReflectiveShadowMapRenderModule.h"

#include "RenderPasses/ReflectiveDirectionalShadowMapAccumulationPass.h"
#include "RenderPasses/ReflectiveShadowMapRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

void ReflectiveShadowMapRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new ReflectiveDirectionalShadowMapAccumulationPass ());
	_renderPasses.push_back (new ReflectiveShadowMapRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
