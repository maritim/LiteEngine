#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Core/Interfaces/Object.h"

class GameModule : public Object
{
public:
	virtual void Init () = 0;
	virtual void UpdateFrame () = 0;
	virtual void UpdateScene () = 0;
	virtual void RenderScene () = 0;
};

#endif