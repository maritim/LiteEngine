#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Core/Math/Vector3.h"

#include <SDL/SDL.h>

class Input
{
private:
	static bool _keyState[256];
	static bool _lastKeyState[256];
	static bool _lastMouseState[4];
	static bool _mouseState[4];
	static bool _sdlQuit;
	static Vector3 _resizeEvent;
public:
	static bool GetButton (SDLKey key);
	static bool GetButtonDown (SDLKey key);
	static bool GetButtonUp (SDLKey key);

	static bool GetKey (unsigned char key);
	static bool GetKeyDown (unsigned char key);
	static bool GetKeyUp (unsigned char key);

	static bool GetMouseButton (Uint8 button);
	static bool GetMouseButtonDown (Uint8 button);
	static bool GetMouseButtonUp (Uint8 button);
	static Vector3 GetMousePosition ();

	static bool GetQuit ();
	static Vector3 GetResizeEvent ();

	static void UpdateState ();
};

#endif