#include "SceneRenderModule.h"

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

#include "RenderPasses/ShadowMap/DirectionalLightShadowMapContainerRenderSubPass.h"
// #include "RenderPasses/ShadowMap/DirectionalLightExponentialShadowMapContainerRenderSubPass.h"
// #include "RenderPasses/ShadowMap/ExponentialShadowMapBlurContainerRenderSubPass.h"
#include "RenderPasses/DeferredDirectionalLightContainerRenderSubPass.h"
#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

#include "RenderPasses/DeferredPointLightContainerRenderSubPass.h"
#include "RenderPasses/PointLightContainerRenderVolumeCollection.h"

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

void SceneRenderModule::Init ()
{
	_renderPasses.push_back (new DeferredGeometryRenderPass ());	
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new DirectionalLightContainerRenderVolumeCollection ())
		.Attach (new DirectionalLightShadowMapContainerRenderSubPass ())
		// .Attach (new DirectionalLightExponentialShadowMapContainerRenderSubPass ())
		// .Attach (new ExponentialShadowMapBlurContainerRenderSubPass ())
		.Attach (new DeferredDirectionalLightContainerRenderSubPass ())
		.Build ());
	_renderPasses.push_back (ContainerRenderPass::Builder ()
		.Volume (new PointLightContainerRenderVolumeCollection ())
		.Attach (new DeferredPointLightContainerRenderSubPass ())
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
	_renderPasses.push_back (new GUIGizmosRenderPass ());
}
