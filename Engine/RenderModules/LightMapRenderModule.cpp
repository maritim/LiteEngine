#include "LightMapRenderModule.h"

#include "RenderPasses/ResultFrameBufferGenerationRenderPass.h"
#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/LightMapDirectionalLightRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

void LightMapRenderModule::Init ()
{
	_renderPasses.push_back (new ResultFrameBufferGenerationRenderPass ());
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new LightMapDirectionalLightRenderPass ())
		.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (new DeferredBlitRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
