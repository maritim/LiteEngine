#include "Input.h"

#include <SDL/SDL.h>

bool Input::_keyState[256];
bool Input::_lastKeyState[256];
bool Input::_lastMouseState[4];
bool Input::_mouseState[4];
bool Input::_sdlQuit (false);
Vector3 Input::_resizeEvent (Vector3::Zero);

void Input::UpdateState ()
{
	for (int i=0;i<255;i++) {
		_lastKeyState [i] = _keyState [i];
	}

	for (int i=1;i<4;i++) {
		_lastMouseState [i] = _mouseState [i];
	}

	_resizeEvent = Vector3::Zero;

	SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
			case SDL_QUIT:
				_sdlQuit = true;
				break;
			case SDL_VIDEORESIZE :
				_resizeEvent.x = event.resize.w;
				_resizeEvent.y = event.resize.h;
				break;
            case SDL_KEYDOWN:
            	_keyState [(int)event.key.keysym.sym] = true;
            	break;
            case SDL_KEYUP:
            	_keyState [(int)event.key.keysym.sym] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
				_mouseState [(int)event.button.button] = true;
				break; 
			case SDL_MOUSEBUTTONUP:
				_mouseState [(int)event.button.button] = false;
				break;
        }
    }
}

bool Input::GetKey (unsigned char ch)
{
	return _keyState [(int)ch];
}

bool Input::GetKeyDown (unsigned char key)
{
	return _keyState [(int)key] && !_lastKeyState [(int)key];
}

bool Input::GetKeyUp (unsigned char key)
{
	return !_keyState [(int)key] && _lastKeyState [(int)key];
}

bool Input::GetMouseButton (Uint8 button)
{
	return _mouseState [button];
}

bool Input::GetMouseButtonDown (Uint8 button)
{
	return _mouseState [button] && !_lastMouseState [button];
}

bool Input::GetMouseButtonUp (Uint8 button)
{
	return !_mouseState [button] && _lastMouseState [button];
}

Vector3 Input::GetMousePosition ()
{
	int x = 0, y = 0;

	SDL_GetMouseState(&x, &y);

	Vector3 position;
	position.x = x;
	position.y = y;

	return position;
}

bool Input::GetQuit ()
{
	return _sdlQuit;
}

Vector3 Input::GetResizeEvent ()
{
	return _resizeEvent;
}