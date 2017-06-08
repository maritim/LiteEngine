#ifndef SHADOWMAPVOLUME_H
#define SHADOWMAPVOLUME_H

#include "Core/Interfaces/Object.h"

#include "SceneNodes/SceneLayer.h"

class ShadowMapVolume : public Object
{
public:
	ShadowMapVolume ();	
	virtual ~ShadowMapVolume ();

	virtual bool Init (std::size_t cascadedLevelsCount) = 0;
	virtual void BindForShadowMapCatch (std::size_t cascadedLevel = 0) = 0;
	virtual void EndDrawing () = 0;

	virtual void BindForReading () = 0;
	virtual void LockShader (int sceneLayers) = 0;
};

#endif