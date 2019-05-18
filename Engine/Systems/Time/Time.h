// Use SDL_GetTicks()
// Probably need to replace this with own time wrapper

#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>

#include <stdint.h>

class ENGINE_API Time
{
private:
	static uint32_t _currentTimeMS;
	static uint32_t _deltaTimeMS;
public:
	static void Init();
	static void UpdateFrame();

	static float GetDeltaTime();
	static float GetTime();

	static unsigned int GetDeltaTimeMS ();
	static unsigned int GetTimeMS ();
	static unsigned int GetElapsedTimeMS ();
};

#endif