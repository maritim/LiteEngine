#ifndef SHADOWMAPVOLUME_H
#define SHADOWMAPVOLUME_H

#include "Core/Interfaces/Object.h"

#include "SceneNodes/SceneLayer.h"

class ShadowMapVolume : public Object
{
public:
	ShadowMapVolume ();	
	virtual ~ShadowMapVolume ();

	virtual bool Init () = 0;
	virtual void BindForShadowMapCatch () = 0;
	virtual void EndDrawing () = 0;

	virtual void BindForLightPass() = 0;
	virtual void LockShader (int sceneLayers) = 0;
};

#endif