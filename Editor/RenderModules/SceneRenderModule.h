#ifndef SCENERENDERMODULE_H
#define SCENERENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class SceneRenderModule : virtual public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(SceneRenderModule)

#endif