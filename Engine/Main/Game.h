#ifndef GAME_H
#define GAME_H

#include "Core/Singleton/Singleton.h"

#include "GameModule.h"

class Game : public Singleton<Game>
{
	friend Singleton<Game>;

private:
	GameModule* _gameModule;

public:
	void Start ();
private:
	Game ();
	~Game ();
	Game (const Game&);
	Game& operator=(const Game&);

	void UpdateScene ();
	void DisplayScene ();

	void LoadGameModule ();
};

#endif