#ifndef SDLMODULE_H
#define SDLMODULE_H

#include <SDL2/SDL.h>

class SDLModule
{
public:
	static void Init ();
	static void Quit ();
};

#endif