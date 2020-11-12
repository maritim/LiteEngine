#include "ReflectiveShadowMappingRenderModule.h"

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

#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOSamplesGenerationRenderPass.h"
#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAONoiseGenerationRenderPass.h"
#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAORenderPass.h"
#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOBlurRenderPass.h"

#include "RenderPasses/AmbientLight/AmbientLightRenderPass.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightViewAccumulationRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMSamplesGenerationRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMInterpolatedIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectSpecularLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMSubsurfaceScatteringRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMAmbientOcclusionRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/ReflectiveShadowMapping/RSMSpotLightAccumulationRenderPass.h"
#include "RenderPasses/DeferredSpotLightRenderPass.h"
#include "RenderPasses/ShadowMap/DeferredSpotLightShadowMapRenderPass.h"
#include "RenderPasses/SpotLightContainerRenderVolumeCollection.h"

#include "RenderPasses/IdleRenderPass.h"
#include "RenderPasses/ScreenSpaceReflections/SSRRenderPass.h"
#include "RenderPasses/ScreenSpaceReflections/SSRAccumulationRenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAARenderPass.h"
#include "RenderPasses/Bloom/BrightExtractionRenderPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomAccumulationRenderPass.h"
#include "RenderPasses/HighDynamicRange/HDRRenderPass.h"
#include "RenderPasses/TextureLUT/TextureLUTRenderPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionRenderPass.h"

void ReflectiveShadowMappingRenderModule::Init ()
{
	/*
	 * Initialize reflective shadow map render module as a collection of 
	 * render passes
	*/

	_renderPasses.push_back (new ResultFrameBufferGenerationRenderPass ());
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	// _renderPasses.push_back (ContainerRenderPass::Builder ()
	// 	.Volume (new IterateOverRenderVolumeCollection (1))
	// 	.Attach (new SSAOSamplesGenerationRenderPass ())
	// 	.Attach (new SSAONoiseGenerationRenderPass ())
	// 	.Attach (new SSAORenderPass ())
	// 	.Attach (new SSAOBlurRenderPass ())
	// 	.Build ());
	_renderPasses.push_back (new AmbientLightRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new RSMDirectionalLightViewAccumulationRenderPass ())
		.Attach (new RSMSamplesGenerationRenderPass ())
		.Attach (new RSMInterpolatedIndirectDiffuseLightRenderPass ())
		.Attach (new RSMIndirectDiffuseLightRenderPass ())
		.Attach (new RSMIndirectSpecularLightRenderPass ())
		.Attach (new RSMSubsurfaceScatteringRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach (new SSAOSamplesGenerationRenderPass ())
			.Attach (new SSAONoiseGenerationRenderPass ())
			.Attach (new RSMAmbientOcclusionRenderPass ())
			.Attach (new SSAOBlurRenderPass ())
			.Build ())
		.Attach (new RSMDirectionalLightRenderPass ())
		.Build ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new SpotLightContainerRenderVolumeCollection ())
		.Attach (new RSMSpotLightAccumulationRenderPass ())
		.Attach (new RSMSamplesGenerationRenderPass ())
		.Attach (new RSMIndirectDiffuseLightRenderPass ())
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
		.Attach	(new TAARenderPass ())
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
