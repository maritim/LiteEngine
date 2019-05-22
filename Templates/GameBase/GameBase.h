#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "Main/GameModule.h"

class GameBase : public GameModule
{
public:
	void Init ();
	void UpdateFrame ();
	void UpdateScene ();
	void RenderScene ();
};

REGISTER_GAME_MODULE(GameBase)

#endif