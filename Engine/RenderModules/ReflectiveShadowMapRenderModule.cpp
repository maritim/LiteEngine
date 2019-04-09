#include "ReflectiveShadowMapRenderModule.h"

#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"
#include "RenderPasses/Editor/EditorRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/ReflectiveShadowMapDirectionalLightAccumulationContainerRenderSubPass.h"
#include "RenderPasses/ShadowMap/DirectionalLightShadowMapContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapSamplesGenerationContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapDirectionalLightContainerRenderSubPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/DeferredAmbientLightContainerRenderSubPass.h"

#include "RenderPasses/IdleContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BrightExtractionContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomAccumulationContainerRenderSubPass.h"
#include "RenderPasses/HighDynamicRange/HDRContainerRenderSubPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionContainerRenderSubPass.h"

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
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new DeferredAmbientLightContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new IdleContainerRenderSubPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach (new BrightExtractionContainerRenderSubPass ())
			.Attach (ContainerRenderPass::Builder ()
				.Volume (new IterateOverRenderVolumeCollection (5))
				.Attach (new BloomHorizontalBlurContainerRenderSubPass ())
				.Attach (new BloomVerticalBlurContainerRenderSubPass ())
				.Build ())
			.Attach (new BloomAccumulationContainerRenderSubPass ())
			.Build ())
		.Attach (new HDRContainerRenderSubPass ())
		.Attach (new GammaCorrectionContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (new DeferredBlitRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
	_renderPasses.push_back (new EditorRenderPass ());
}
