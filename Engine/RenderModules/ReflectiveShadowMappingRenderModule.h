#ifndef REFLECTIVESHADOWMAPPINGRENDERMODULE_H
#define REFLECTIVESHADOWMAPPINGRENDERMODULE_H

#include "Renderer/RenderModule.h"

#include "Renderer/RenderModuleManager.h"

class ReflectiveShadowMappingRenderModule : public RenderModule
{
protected:
	void Init ();
};

REGISTER_RENDER_MODULE(ReflectiveShadowMappingRenderModule)

#endif