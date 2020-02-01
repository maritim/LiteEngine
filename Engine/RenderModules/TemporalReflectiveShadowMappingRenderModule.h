#ifndef TEMPORALREFLECTIVESHADOWMAPPINGRENDERMODULE_H
#define TEMPORALREFLECTIVESHADOWMAPPINGRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class TemporalReflectiveShadowMappingRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(TemporalReflectiveShadowMappingRenderModule)

#endif