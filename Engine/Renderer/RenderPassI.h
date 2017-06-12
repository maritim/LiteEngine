#ifndef RENDERPASSI_H
#define RENDERPASSI_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "RenderVolumeCollection.h"

#include "SceneGraph/Scene.h"
#include "Systems/Camera/Camera.h"

class RenderPassI : public Object
{
public:
	virtual ~RenderPassI () = 0;

	virtual void Init () = 0;
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc) = 0;
};

#endif