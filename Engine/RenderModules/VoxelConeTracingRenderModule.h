#ifndef VOXELCONETRACINGREDRENDERMODULE_H
#define VOXELCONETRACINGREDRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class VoxelConeTracingRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(VoxelConeTracingRenderModule)

#endif