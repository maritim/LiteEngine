#ifndef SHADOWMAPVOLUME_H
#define SHADOWMAPVOLUME_H

#include "Core/Interfaces/Object.h"

class ShadowMapVolume : public Object
{
public:
	ShadowMapVolume ();	
	virtual ~ShadowMapVolume ();

	virtual bool Init () = 0;
	virtual void BindForShadowMapCatch () = 0;
	virtual void EndDrawing () = 0;
};

#endif