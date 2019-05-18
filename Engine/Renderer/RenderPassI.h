#ifndef RENDERPASSI_H
#define RENDERPASSI_H

#include "Core/Interfaces/Object.h"

#include "RenderVolumeCollection.h"

#include "SceneGraph/Scene.h"
#include "Systems/Camera/Camera.h"
#include "RenderSettings.h"

class ENGINE_API RenderPassI : public Object
{
public:
	virtual ~RenderPassI () = 0;

	virtual void Init (const RenderSettings&) = 0;
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc) = 0;

	virtual void Clear () = 0;
};

#endif