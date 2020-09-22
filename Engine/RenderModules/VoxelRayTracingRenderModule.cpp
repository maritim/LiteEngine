#include "VoxelRayTracingRenderModule.h"

#include "RenderPasses/ResultFrameBufferGenerationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelGenerationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelizationRenderPass.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightAccumulationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelRadianceInjectionRenderPass.h"
#include "RenderPasses/Voxelization/VoxelMipmapRenderPass.h"
#include "RenderPasses/Voxelization/VoxelBorderRenderPass.h"
#include "RenderPasses/VoxelRayTracing/VRTRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"
#include "RenderPasses/GUI/GUIGizmosRenderPass.h"
#include "RenderPasses/GUI/GUIRenderPass.h"
#include "RenderPasses/WindowBlitRenderPass.h"

#include "RenderPasses/Container/ContainerRenderPass.h"
#include "RenderPasses/IterateOverRenderVolumeCollection.h"

#include "RenderPasses/VoxelConeTracing/VCTVoxelizationCheckRenderVolumeCollection.h"

#include "RenderPasses/DirectionalLightContainerRenderVolumeCollection.h"

void VoxelRayTracingRenderModule::Init ()
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
		.Build ());
	_renderPasses.push_back (new VRTRenderPass ());
	_renderPasses.push_back (new WindowBlitRenderPass ());
	_renderPasses.push_back (new GUIGizmosRenderPass ());
	_renderPasses.push_back (new GUIRenderPass ());
}
