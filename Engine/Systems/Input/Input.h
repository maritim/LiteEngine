#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Core/Math/glm/vec3.hpp"

#include <SDL2/SDL.h>

/*
 * Maybe change this into a Hash Table?
*/

#define KEYS_COUNT (1<<8)

class Input
{
private:
	static bool _keyState[KEYS_COUNT];
	static bool _lastKeyState[KEYS_COUNT];
	static bool _lastMouseState[4];
	static bool _mouseState[4];
	static bool _sdlQuit;
	static glm::vec3 _resizeEvent;
public:
	static bool GetButton (SDL_Keycode key);
	static bool GetButtonDown (SDL_Keycode key);
	static bool GetButtonUp (SDL_Keycode key);

	static bool GetKey (unsigned char key);
	static bool GetKeyDown (unsigned char key);
	static bool GetKeyUp (unsigned char key);

	static bool GetMouseButton (Uint8 button);
	static bool GetMouseButtonDown (Uint8 button);
	static bool GetMouseButtonUp (Uint8 button);
	static glm::vec3 GetMousePosition ();

	static bool GetQuit ();
	static glm::vec3 GetResizeEvent ();

	static void UpdateState ();
};

#endif