#include "VoxelizationRenderModule.h"

#include <vector>
#include <algorithm>

#include "VoxelizationRenderPass.h"
#include "DirectionalShadowMapRenderPass.h"
#include "VoxelRadianceInjectionRenderPass.h"
#include "VoxelMipmapRenderPass.h"
#include "VoxelRayTraceRenderPass.h"
#include "ForwardRenderPass.h"

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
