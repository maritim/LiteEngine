#include "DeferredRenderModule.h"

#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredLightRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

void DeferredRenderModule::Init ()
{
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (new DeferredLightRenderPass ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (new DeferredBlitRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
