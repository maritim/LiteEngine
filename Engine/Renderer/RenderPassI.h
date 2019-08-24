#ifndef RENDERPASSI_H
#define RENDERPASSI_H

#include "Core/Interfaces/Object.h"

#include "RenderVolumeCollection.h"

#include "Renderer/RenderScene.h"
#include "Systems/Camera/Camera.h"
#include "RenderSettings.h"

#define DECLARE_RENDER_PASS(T) \
public: \
	std::string GetName () const { return #T; }

class ENGINE_API RenderPassI : public Object
{
public:
	virtual ~RenderPassI () = 0;

	virtual void Init (const RenderSettings&) = 0;
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc) = 0;

	virtual std::string GetName () const = 0;

	virtual void Clear () = 0;
};

#endif