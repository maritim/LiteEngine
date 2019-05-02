#ifndef EDITORRENDERMODULE_H
#define EDITORRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class EditorRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(EditorRenderModule)

#endif