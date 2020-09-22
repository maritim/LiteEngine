#include "VoxelConeTracingRenderModule.h"

#include "RenderPasses/ResultFrameBufferGenerationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelGenerationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelizationRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightAccumulationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelRadianceInjectionRenderPass.h"
#include "RenderPasses/Voxelization/VoxelMipmapRenderPass.h"
#include "RenderPasses/Voxelization/VoxelBorderRenderPass.h"
#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"
#include "RenderPasses/GUI/GUIGizmosRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/VoxelConeTracing/VCTVoxelizationCheckRenderVolumeCollection.h"
#include "RenderPasses/VoxelConeTracing/VCTDebugCheckRenderVolumeCollection.h"

// #include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOSamplesGenerationRenderPass.h"
// #include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAONoiseGenerationRenderPass.h"
// #include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAORenderPass.h"
// #include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOBlurRenderPass.h"

#include "RenderPasses/AmbientLight/AmbientLightRenderPass.h"

#include "RenderPasses/VoxelConeTracing/VCTDirectionalLightRenderPass.h"
#include "RenderPasses/VoxelConeTracing/VCTIndirectDiffuseLightRenderPass.h"
#include "RenderPasses/VoxelConeTracing/VCTIndirectSpecularLightRenderPass.h"
#include "RenderPasses/VoxelConeTracing/VCTAmbientOcclusionRenderPass.h"
#include "RenderPasses/VoxelConeTracing/VCTSubsurfaceScatteringRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/IdleRenderPass.h"
#include "RenderPasses/ScreenSpaceReflections/SSRRenderPass.h"
#include "RenderPasses/ScreenSpaceReflections/SSRAccumulationRenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAARenderPass.h"
#include "RenderPasses/TemporalAntialiasing/TAASwapRenderPass.h"
#include "RenderPasses/Bloom/BrightExtractionRenderPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurRenderPass.h"
#include "RenderPasses/Bloom/BloomAccumulationRenderPass.h"
#include "RenderPasses/HighDynamicRange/HDRRenderPass.h"
#include "RenderPasses/TextureLUT/TextureLUTRenderPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionRenderPass.h"

void VoxelConeTracingRenderModule::Init ()
{
	/*
	* Initialize voxel cone trace render module as a collection of render passes
	*/

	_renderPasses.push_back (new ResultFrameBufferGenerationRenderPass ());
	_renderPasses.push_back (new VoxelGenerationRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new VCTVoxelizationCheckRenderVolumeCollection ())
		.Attach (new VoxelizationRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new DirectionalLightContainerRenderVolumeCollection ())
			.Attach (new RSMDirectionalLightAccumulationRenderPass ())
			.Attach (new VoxelRadianceInjectionRenderPass ())
			.Build ())
		.Attach (new VoxelMipmapRenderPass ())
		// .Attach (new VoxelBorderRenderPass ())
		.Build ());
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
		// .Attach (ContainerRenderPass::Builder ()
		// 	.Volume (new IterateOverRenderVolumeCollection (1))
		// 	.Attach (new SSAOSamplesGenerationRenderPass ())
		// 	.Attach (new SSAONoiseGenerationRenderPass ())
		// 	.Attach (new SSAORenderPass ())
		// 	.Attach (new SSAOBlurRenderPass ())
		// 	.Build ())
	_renderPasses.push_back (new AmbientLightRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new VCTIndirectDiffuseLightRenderPass ())
		.Attach (new VCTIndirectSpecularLightRenderPass ())
		.Attach (new VCTAmbientOcclusionRenderPass ())
		.Attach (new VCTSubsurfaceScatteringRenderPass ())
		.Attach (new VCTDirectionalLightRenderPass ())
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
	_renderPasses.push_back (new GUIGizmosRenderPass ());
}
