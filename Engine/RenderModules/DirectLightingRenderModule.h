#ifndef DIRECTRENDERMODULE_H
#define DIRECTRENDERMODULE_H

#include "Renderer/RenderModule.h"

class DirectLightingRenderModule : virtual public RenderModule
{
protected:
	void Init ();
};

#endif