#include "VoxelConeTraceRenderModule.h"

#include "RenderPasses/VoxelizationRenderPass.h"
#include "RenderPasses/VoxelDirectionalShadowMapRenderPass.h"
#include "RenderPasses/VoxelRadianceInjectionRenderPass.h"
#include "RenderPasses/VoxelMipmapRenderPass.h"
#include "RenderPasses/VoxelBorderRenderPass.h"
#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredSkyboxRenderPass.h"
#include "RenderPasses/DeferredBlitRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/ShadowMap/DirectionalLightShadowMapContainerRenderSubPass.h"
#include "RenderPasses/VoxelConeTraceDirectionalLightRenderPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/DeferredAmbientLightContainerRenderSubPass.h"

#include "RenderPasses/IdleContainerRenderSubPass.h"
#include "RenderPasses/ScreenSpaceReflection/SSRContainerRenderSubPass.h"
#include "RenderPasses/ScreenSpaceReflection/SSRAccumulationContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BrightExtractionContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomHorizontalBlurContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomVerticalBlurContainerRenderSubPass.h"
#include "RenderPasses/Bloom/BloomAccumulationContainerRenderSubPass.h"
#include "RenderPasses/HighDynamicRange/HDRContainerRenderSubPass.h"
#include "RenderPasses/GammaCorrection/GammaCorrectionContainerRenderSubPass.h"

void VoxelConeTraceRenderModule::Init ()
{
	/*
	* Initialize voxel cone trace render module as a collection of render passes
	*/

	_renderPasses.push_back (new VoxelizationRenderPass ());
	_renderPasses.push_back (new VoxelDirectionalShadowMapRenderPass ());
	_renderPasses.push_back (new VoxelRadianceInjectionRenderPass ());
	_renderPasses.push_back (new VoxelMipmapRenderPass ());
	_renderPasses.push_back (new VoxelBorderRenderPass ());
	_renderPasses.push_back (new DeferredGeometryRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new DirectionalLightShadowMapContainerRenderSubPass ())
		.Attach (new VoxelConeTraceDirectionalLightRenderPass ())
		.Build ());
	// _renderPasses.push_back (ContainerRenderPass::Builder ()
	// 	.Volume (new IterateOverRenderVolumeCollection (1))
	// 	.Attach (new DeferredAmbientLightContainerRenderSubPass ())
	// 	.Build ());
	_renderPasses.push_back (new DeferredSkyboxRenderPass ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new IterateOverRenderVolumeCollection (1))
		.Attach (new IdleContainerRenderSubPass ())
		.Attach (ContainerRenderPass::Builder ()
			.Volume (new IterateOverRenderVolumeCollection (1))
			.Attach (new SSRContainerRenderSubPass ())
			.Attach (new SSRAccumulationContainerRenderSubPass ())
			.Build ())
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
}
