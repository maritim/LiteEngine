#ifndef TEMPORALSCREENSPACEDIRECTIONALOCCLUSIONRENDERMODULE_H
#define TEMPORALSCREENSPACEDIRECTIONALOCCLUSIONRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class TemporalScreenSpaceDirectionalOcclusionRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(TemporalScreenSpaceDirectionalOcclusionRenderModule)

#endif