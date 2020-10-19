#include "HybridGlobalIlluminationRenderModule.h"

#include "RenderPasses/ResultFrameBufferGenerationRenderPass.h"
#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"
#include "RenderPasses/GUI/GUIGizmosRenderPass.h"
#include "RenderPasses/GUI/GUIRenderPass.h"
#include "RenderPasses/WindowBlitRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAONoiseGenerationRenderPass.h"

#include "RenderPasses/AmbientLight/AmbientLightRenderPass.h"

#include "RenderPasses/DeferredDirectionalLightRenderPass.h"

#include "RenderPasses/FramebufferGenerationRenderPass.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightViewAccumulationRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HGISamplesGenerationRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridRSMInterpolatedIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridRSMIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HGIDirectLightDirectionalRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridSSDOInterpolatedIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridSSDOIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridSSRGenerationRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HGIIndirectSpecularLightRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridAOSamplesGenerationRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridRSMAmbientOcclusionRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridAmbientOcclusionRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HybridAmbientOcclusionBlurRenderPass.h"
#include "RenderPasses/HybridGlobalIllumination/HGIRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMSamplesGenerationRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMInterpolatedIndirectDiffuseLightRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMIndirectDiffuseLightRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMIndirectSpecularLightRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMSubsurfaceScatteringRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMAmbientOcclusionRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightRenderPass.h"
// #include "RenderPasses/ReflectiveShadowMapping/RSMRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMSpotLightAccumulationRenderPass.h"
#include "RenderPasses/DeferredSpotLightRenderPass.h"
#include "RenderPasses/ShadowMap/DeferredSpotLightShadowMapRenderPass.h"
#include "RenderPasses/SpotLightContainerRenderVolumeCollection.h"

#include "RenderPasses/IdleRenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAARenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAASwapRenderPass.h"
#include "RenderPasses/Bloom/BrightExtractionRenderPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomAccumulationRenderPass.h"
#include "RenderPasses/HighDynamicRange/HDRRenderPass.h"
#include "RenderPasses/TextureLUT/TextureLUTRenderPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionRenderPass.h"

void HybridGlobalIlluminationRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new ResultFrameBufferGenerationRenderPass ());
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (new AmbientLightRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new FramebufferGenerationRenderPass ("directLightMap"))
		.Attach (new FramebufferGenerationRenderPass ("indirectSpecularMap"))
		.Attach (new FramebufferGenerationRenderPass ("ambientOcclusionMap"))
		.Attach (new HybridAOSamplesGenerationRenderPass ())
		.Attach (new SSAONoiseGenerationRenderPass ())
		.Attach (new HybridSSRGenerationRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new DirectionalLightContainerRenderVolumeCollection ())
			.Attach (new RSMDirectionalLightViewAccumulationRenderPass ())
			.Attach (new HGISamplesGenerationRenderPass ())
			.Attach (new HybridRSMInterpolatedIndirectDiffuseLightRenderPass ())
			.Attach (new HybridRSMIndirectDiffuseLightRenderPass ())
			.Attach (new HGIDirectLightDirectionalRenderPass ())
			.Attach (new HGIIndirectSpecularLightRenderPass ())
			.Attach (new HybridRSMAmbientOcclusionRenderPass ())
			.Build ())
		.Attach (new HybridSSDOInterpolatedIndirectDiffuseLightRenderPass ())
		.Attach (new HybridSSDOIndirectDiffuseLightRenderPass ())
		.Attach (new HybridAmbientOcclusionRenderPass ())
		.Attach (new HybridAmbientOcclusionBlurRenderPass ())
		.Attach (new HGIRenderPass ())
		.Build ());
		// .Attach (new RSMInterpolatedIndirectDiffuseLightRenderPass ())
		// .Attach (new RSMIndirectDiffuseLightRenderPass ())
		// .Attach (new RSMIndirectSpecularLightRenderPass ())
		// .Attach (new RSMSubsurfaceScatteringRenderPass ())
	// _renderPasses.push_back (ContainerRenderPass::Builder ()
	// 	.Volume (new SpotLightContainerRenderVolumeCollection ())
	// 	.Attach (new RSMSpotLightAccumulationRenderPass ())
	// 	.Attach (new RSMSamplesGenerationRenderPass ())
	// 	.Attach (new RSMIndirectDiffuseLightRenderPass ())
	// 	.Attach (new RSMIndirectSpecularLightRenderPass ())
	// 	.Attach (new DeferredSpotLightRenderPass ())
	// 	.Attach (new RSMRenderPass ())
	// 	.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new IdleRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach	(new TAARenderPass ())
			.Attach (new TAASwapRenderPass ())
			.Build ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach (new BrightExtractionRenderPass ())
			.Attach (ContainerRenderPass::Builder ()
				.Volume (new IterateOverRenderVolumeCollection (5))
				.Attach (new BloomHorizontalBlurRenderPass ())
				.Attach (new BloomVerticalBlurRenderPass ())
				.Build ())
			.Attach (new BloomAccumulationRenderPass ())
			.Build ())
		.Attach (new HDRRenderPass ())
		.Attach (new TextureLUTRenderPass ())
		.Attach (new GammaCorrectionRenderPass ())
		.Attach (new DeferredBlitRenderPass ())
		.Build ());
	_renderPasses.push_back (new ForwardRenderPass ());
	_renderPasses.push_back (new WindowBlitRenderPass());
	_renderPasses.push_back (new GUIGizmosRenderPass ());
	_renderPasses.push_back (new GUIRenderPass ());
}
