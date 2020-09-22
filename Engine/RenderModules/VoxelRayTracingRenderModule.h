#ifndef VOXELRAYTRACINGREDRENDERMODULE_H
#define VOXELRAYTRACINGREDRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class VoxelRayTracingRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(VoxelRayTracingRenderModule)

#endif