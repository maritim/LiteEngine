#include "Time.h"

uint32_t Time::_currentTimeMS (0);
uint32_t Time::_deltaTimeMS (0);

float Time::GetDeltaTime()
{
	return _deltaTimeMS / 1000.0f;
}

float Time::GetTime()
{
	return _currentTimeMS / 1000.0f;
}

void Time::Init() 
{
	_currentTimeMS = SDL_GetTicks();
}

#include <algorithm>

void Time::UpdateFrame()
{
	uint32_t lastTimeMS = _currentTimeMS;
	_currentTimeMS = SDL_GetTicks();

	//TODO: Fix this
	_deltaTimeMS = std::max (1u, _currentTimeMS - lastTimeMS);
}

unsigned int Time::GetDeltaTimeMS ()
{
	return _deltaTimeMS;
}

/*
 * Get total milliseconds passed since the start of game until the start of
 * current frame. 
 *
 * It is possible that more milliseconds already pass since the starting of
 * frame.
*/

unsigned int Time::GetTimeMS ()
{
	return _currentTimeMS;
}

/* 
 * Get all miliseconds passed since the start of game.
*/

unsigned int Time::GetElapsedTimeMS ()
{
	return SDL_GetTicks ();
}