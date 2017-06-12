#ifndef RENDERMODULE_H
#define RENDERMODULE_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "RenderPassI.h"

#include "SceneGraph/Scene.h"
#include "Systems/Camera/Camera.h"

class RenderModule : public Object
{
protected:
	std::vector<RenderPassI*> _renderPasses;

public:
	virtual ~RenderModule ();

	virtual void InitModule ();
	virtual void RenderScene (Scene*, Camera*);
protected:
	virtual void Init () = 0;
};

#endif