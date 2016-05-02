#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class GameEngine
{
public:
	static void Init ();
	static void Clear ();
private:
	static void InitOpenGL ();
	static void InitScene ();
};

#endif