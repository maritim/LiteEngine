#include "SDLModule.h"

#include <string>

#include "Core/Console/Console.h"

/*
 * TODO: Take this from a configure file.
*/

#define SDL_INIT_FLAGS SDL_INIT_VIDEO | SDL_INIT_JOYSTICK

/*
 * TODO: Take this from another part of code.
*/

#define EXIT_CODE_SDL_FAIL 33

void SDLModule::Init ()
{
	if (SDL_Init(SDL_INIT_FLAGS) < 0) {
		std::string errorMessage = SDL_GetError ();

		Console::LogError ("Failed to init SDL 2.0.");
		Console::LogError ("Error: " + errorMessage);
		Console::LogError ("Aborting...");

		exit (EXIT_CODE_SDL_FAIL);
	} else {
		Console::Log ("SDL 2.0 is ready.");
	}
}

void SDLModule::Quit ()
{
	SDL_Quit ();
}
