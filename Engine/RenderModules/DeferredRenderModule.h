#ifndef DEFERREDRENDERMODULE_H
#define DEFERREDRENDERMODULE_H

#include "Renderer/RenderModule.h"

class DeferredRenderModule : virtual public RenderModule
{
protected:
	void Init ();
};

#endif