#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Core/Interfaces/Object.h"

#ifdef _WIN32
	#define REGISTER_GAME_MODULE(module) \
	extern "C" __declspec(dllexport) module* CreateGameModule () \
	{ \
		return new module; \
	} \
	extern "C" __declspec(dllexport) void DestroyGameModule (module* gameModule) \
	{ \
		delete gameModule; \
	}
#else
	#define REGISTER_GAME_MODULE(module) \
	extern "C" module* CreateGameModule () \
	{ \
		return new module; \
	} \
	extern "C" void DestroyGameModule (module* gameModule) \
	{ \
		delete gameModule; \
	}
#endif

class GameModule : public Object
{
public:
	virtual void Init () = 0;
	virtual void UpdateFrame () = 0;
	virtual void UpdateScene () = 0;
	virtual void RenderScene () = 0;
};

#endif