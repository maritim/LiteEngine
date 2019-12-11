#include "VoxelConeTracingRenderModule.h"

#include "RenderPasses/ResultFrameBufferGenerationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelizationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelDirectionalShadowMapRenderPass.h"
#include "RenderPasses/Voxelization/VoxelRadianceInjectionRenderPass.h"
#include "RenderPasses/Voxelization/VoxelMipmapRenderPass.h"
#include "RenderPasses/Voxelization/VoxelBorderRenderPass.h"
#include "RenderPasses/VoxelRayTracing/VRTRenderPass.h"
#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"
#include "RenderPasses/GUI/GUIGizmosRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/VoxelConeTracing/VCTDebugCheckRenderVolumeCollection.h"

#include "RenderPasses/AmbientOcclusion/SSAOSamplesGenerationRenderPass.h"
#include "RenderPasses/AmbientOcclusion/SSAONoiseGenerationRenderPass.h"
#include "RenderPasses/AmbientOcclusion/SSAORenderPass.h"
#include "RenderPasses/AmbientOcclusion/SSAOBlurRenderPass.h"

#include "RenderPasses/AmbientLight/AmbientLightRenderPass.h"

#include "RenderPasses/VoxelConeTracing/VCTDirectionalLightRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

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
#include "RenderPasses/GammaCorrection/GammaCorrectionRenderPass.h"

void VoxelConeTracingRenderModule::Init ()
{
	/*
	* Initialize voxel cone trace render module as a collection of render passes
	*/

	_renderPasses.push_back (new ResultFrameBufferGenerationRenderPass ());
	_renderPasses.push_back (new VoxelizationRenderPass ());
	_renderPasses.push_back (new VoxelDirectionalShadowMapRenderPass ());
	_renderPasses.push_back (new VoxelRadianceInjectionRenderPass ());
	_renderPasses.push_back (new VoxelMipmapRenderPass ());
	_renderPasses.push_back (new VoxelBorderRenderPass ());
	_renderPasses.push_back (new VRTRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new VCTDebugCheckRenderVolumeCollection ())
		.Attach (new DeferredGeometryRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach (new SSAOSamplesGenerationRenderPass ())
			.Attach (new SSAONoiseGenerationRenderPass ())
			.Attach (new SSAORenderPass ())
			.Attach (new SSAOBlurRenderPass ())
			.Build ())
		.Attach (new AmbientLightRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new DirectionalLightContainerRenderVolumeCollection ())
			.Attach (new VCTDirectionalLightRenderPass ())
			.Build ())
		.Attach (new DeferredSkyboxRenderPass ())
		.Attach (ContainerRenderPass::Builder ()
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
			.Attach (new GammaCorrectionRenderPass ())
			.Attach (new DeferredBlitRenderPass ())
			.Build ())
		.Attach (new ForwardRenderPass ())
		.Attach (new GUIGizmosRenderPass ())
		.Build ());
}
