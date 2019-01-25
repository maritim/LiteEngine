#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class GameEngine
{
public:
	static void Init ();
	static void Clear ();
private:
	static void InitSettings ();
	static void InitOpenGL ();
	static void InitRenderer ();
	static void InitScene ();
};

#endif