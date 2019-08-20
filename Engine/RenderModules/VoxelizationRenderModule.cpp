#include "VoxelizationRenderModule.h"

#include "RenderPasses/Voxelization/VoxelizationRenderPass.h"
#include "RenderPasses/Voxelization/VoxelDirectionalShadowMapRenderPass.h"
#include "RenderPasses/Voxelization/VoxelRadianceInjectionRenderPass.h"
#include "RenderPasses/Voxelization/VoxelMipmapRenderPass.h"
#include "RenderPasses/Voxelization/VoxelBorderRenderPass.h"
#include "RenderPasses/VoxelRayTraceRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

void VoxelizationRenderModule::Init ()
{
	/*
	 * Initialize voxelization render module as a collection of render passes
	*/

	_renderPasses.push_back (new VoxelizationRenderPass ());
	_renderPasses.push_back (new VoxelDirectionalShadowMapRenderPass ());
	_renderPasses.push_back (new VoxelRadianceInjectionRenderPass ());
	_renderPasses.push_back (new VoxelMipmapRenderPass ());
	_renderPasses.push_back (new VoxelBorderRenderPass ());
	_renderPasses.push_back (new VoxelRayTraceRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
