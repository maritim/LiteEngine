#include "Input.h"

bool Input::_keyState[256];
bool Input::_lastKeyState[256];
bool Input::_lastMouseState[4];
bool Input::_mouseState[4];
bool Input::_sdlQuit (false);
glm::vec3 Input::_resizeEvent (glm::vec3 (0.0f));

void Input::UpdateState ()
{
	/*
	 * TODO: Change this with a map
	*/

	for (int i=0;i<KEYS_COUNT;i++) {
		_lastKeyState [i] = _keyState [i];
	}

	for (int i=1;i<4;i++) {
		_lastMouseState [i] = _mouseState [i];
	}

	_resizeEvent = glm::vec3 (0.0f);

	SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
			case SDL_QUIT:
				_sdlQuit = true;
				break;
	        case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					_resizeEvent.x = event.window.data1;
					_resizeEvent.y = event.window.data2;
				}
				break;
            case SDL_KEYDOWN:
				if ((int)event.key.keysym.sym >= KEYS_COUNT) {
					break;
				}
				_keyState[(int)event.key.keysym.sym] = true;
            	break;
            case SDL_KEYUP:
				if ((int)event.key.keysym.sym >= KEYS_COUNT) {
					break;
				}
				_keyState[(int)event.key.keysym.sym] = false;
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

glm::vec3 Input::GetMousePosition ()
{
	int x = 0, y = 0;

	SDL_GetMouseState(&x, &y);

	glm::vec3 position;
	position.x = x;
	position.y = y;

	return position;
}

bool Input::GetQuit ()
{
	return _sdlQuit;
}

glm::vec3 Input::GetResizeEvent ()
{
	return _resizeEvent;
}