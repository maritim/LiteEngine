#include "ReflectiveShadowMappingRenderModule.h"

#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"
#include "RenderPasses/GUI/GUIGizmosRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/AmbientOcclusion/SSAOSamplesGenerationContainerRenderSubPass.h"
#include "RenderPasses/AmbientOcclusion/SSAONoiseGenerationContainerRenderSubPass.h"
#include "RenderPasses/AmbientOcclusion/SSAOContainerRenderSubPass.h"
#include "RenderPasses/AmbientOcclusion/SSAOBlurContainerRenderSubPass.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightAccumulationContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMSamplesGenerationContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightContainerRenderSubPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMCachingContainerRenderSubPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/DeferredAmbientLightContainerRenderSubPass.h"

#include "RenderPasses/IdleContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BrightExtractionContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomAccumulationContainerRenderSubPass.h"
#include "RenderPasses/HighDynamicRange/HDRContainerRenderSubPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionContainerRenderSubPass.h"

void ReflectiveShadowMappingRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new RSMDirectionalLightAccumulationContainerRenderSubPass ())
		.Attach (new RSMSamplesGenerationContainerRenderSubPass ())
		.Attach (new RSMCachingContainerRenderSubPass ())
		.Attach (new RSMDirectionalLightContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new SSAOSamplesGenerationContainerRenderSubPass ())
		.Attach (new SSAONoiseGenerationContainerRenderSubPass ())
		.Attach (new SSAOContainerRenderSubPass ())
		.Attach (new SSAOBlurContainerRenderSubPass ())
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
	_renderPasses.push_back (new GUIGizmosRenderPass ());
}
