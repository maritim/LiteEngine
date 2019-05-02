#ifndef DIRECTRENDERMODULE_H
#define DIRECTRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class DirectLightingRenderModule : virtual public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(DirectLightingRenderModule)

#endif