#ifndef SCREENSPACEDIRECTIONALOCCLUSIONRENDERMODULE_H
#define SCREENSPACEDIRECTIONALOCCLUSIONRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class ScreenSpaceDirectionalOcclusionRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(ScreenSpaceDirectionalOcclusionRenderModule)

#endif