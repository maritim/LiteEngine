#include "TemporalReflectiveShadowMappingRenderModule.h"

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

#include "RenderPasses/AmbientOcclusion/SSAOSamplesGenerationRenderPass.h"
#include "RenderPasses/AmbientOcclusion/SSAONoiseGenerationRenderPass.h"
#include "RenderPasses/AmbientOcclusion/SSAORenderPass.h"
#include "RenderPasses/AmbientOcclusion/SSAOBlurRenderPass.h"

#include "RenderPasses/AmbientLight/AmbientLightRenderPass.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightViewAccumulationRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectSpecularLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMSubsurfaceScatteringRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMRenderPass.h"

#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMSamplesGenerationRenderPass.h"
#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMTemporalFilterRenderPass.h"
#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMMedianFilterRenderPass.h"
#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMBlurRenderPass.h"
#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMIndirectSwapRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMSpotLightAccumulationRenderPass.h"
#include "RenderPasses/DeferredSpotLightRenderPass.h"
#include "RenderPasses/ShadowMap/DeferredSpotLightShadowMapRenderPass.h"
#include "RenderPasses/SpotLightContainerRenderVolumeCollection.h"

#include "RenderPasses/IdleRenderPass.h"
#include "RenderPasses/ScreenSpaceReflection/SSRRenderPass.h"
#include "RenderPasses/ScreenSpaceReflection/SSRAccumulationRenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAARenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAASwapRenderPass.h"
#include "RenderPasses/Bloom/BrightExtractionRenderPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomAccumulationRenderPass.h"
#include "RenderPasses/HighDynamicRange/HDRRenderPass.h"
#include "RenderPasses/TextureLUT/TextureLUTRenderPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionRenderPass.h"

void TemporalReflectiveShadowMappingRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new ResultFrameBufferGenerationRenderPass ());
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new SSAOSamplesGenerationRenderPass ())
		.Attach (new SSAONoiseGenerationRenderPass ())
		.Attach (new SSAORenderPass ())
		.Attach (new SSAOBlurRenderPass ())
		.Build ());
	_renderPasses.push_back (new AmbientLightRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new RSMDirectionalLightViewAccumulationRenderPass ())
		.Attach (new TRSMSamplesGenerationRenderPass ())
		.Attach (new TRSMIndirectDiffuseLightRenderPass ())
		.Attach (new TRSMTemporalFilterRenderPass ())
		.Attach (new TRSMMedianFilterRenderPass ())
		.Attach (new TRSMBlurRenderPass ())
		.Attach (new TRSMIndirectSwapRenderPass ())
		.Attach (new RSMIndirectSpecularLightRenderPass ())
		.Attach (new RSMSubsurfaceScatteringRenderPass ())
		.Attach (new RSMDirectionalLightRenderPass ())
		.Build ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new SpotLightContainerRenderVolumeCollection ())
		.Attach (new RSMSpotLightAccumulationRenderPass ())
		.Attach (new TRSMSamplesGenerationRenderPass ())
		.Attach (new TRSMIndirectDiffuseLightRenderPass ())
		.Attach (new TRSMTemporalFilterRenderPass ())
		.Attach (new TRSMMedianFilterRenderPass ())
		.Attach (new TRSMBlurRenderPass ())
		.Attach (new TRSMIndirectSwapRenderPass ())
		.Attach (new RSMIndirectSpecularLightRenderPass ())
		.Attach (new DeferredSpotLightRenderPass ())
		.Attach (new RSMRenderPass ())
		.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new IdleRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach (new SSRRenderPass ())
			.Attach (new SSRAccumulationRenderPass ())
			.Build ())
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
