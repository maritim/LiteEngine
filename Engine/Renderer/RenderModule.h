#ifndef RENDERMODULE_H
#define RENDERMODULE_H

#include "Core/Interfaces/Object.h"

#include "SceneGraph/Scene.h"
#include "Systems/Camera/Camera.h"

class RenderModule : public Object
{
public:
	virtual ~RenderModule () = 0;

	virtual void RenderScene (Scene*, Camera*) = 0;
};

#endif