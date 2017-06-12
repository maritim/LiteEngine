#include "VoxelizationRenderModule.h"

#include <vector>
#include <algorithm>

#include "RenderPasses/VoxelizationRenderPass.h"
#include "RenderPasses/DirectionalShadowMapRenderPass.h"
#include "RenderPasses/VoxelRadianceInjectionRenderPass.h"
#include "RenderPasses/VoxelMipmapRenderPass.h"
#include "RenderPasses/VoxelRayTraceRenderPass.h"
#include "RenderPasses/ForwardRenderPass.h"

void VoxelizationRenderModule::Init ()
{
	/*
	 * Initialize voxelization render module as a collection of render passes
	*/

	_renderPasses.push_back (new VoxelizationRenderPass ());
	_renderPasses.push_back (new DirectionalShadowMapRenderPass ());
	_renderPasses.push_back (new VoxelRadianceInjectionRenderPass ());
	_renderPasses.push_back (new VoxelMipmapRenderPass ());
	_renderPasses.push_back (new VoxelRayTraceRenderPass ());
	_renderPasses.push_back (new ForwardRenderPass ());
}
