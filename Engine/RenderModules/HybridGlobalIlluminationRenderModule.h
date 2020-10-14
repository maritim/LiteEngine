#ifndef HYBRIDGLOBALILLUMINATIONRENDERMODULE_H
#define HYBRIDGLOBALILLUMINATIONRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class HybridGlobalIlluminationRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(HybridGlobalIlluminationRenderModule)

#endif