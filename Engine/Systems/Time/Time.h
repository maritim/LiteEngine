// Use SDL_GetTicks()
// Probably need to replace this with own time wrapper

#ifndef TIME_H
#define TIME_H

#include <SDL/SDL.h>

class Time
{
private:
	static Uint32 _currentTimeMS;
	static Uint32 _deltaTimeMS;
public:
	static void Init();
	static void UpdateFrame();

	static float GetDeltaTime();
	static float GetTime();

	static unsigned int GetDeltaTimeMS ();
};

#endif