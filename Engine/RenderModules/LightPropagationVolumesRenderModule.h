#ifndef LIGHTPROPAGATIONVOLUMESRENDERMODULE_H
#define LIGHTPROPAGATIONVOLUMESRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class LightPropagationVolumesRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(LightPropagationVolumesRenderModule)

#endif