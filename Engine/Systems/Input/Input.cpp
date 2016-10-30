#include "Input.h"

std::map<int, bool> Input::_keyState;
std::map<int, bool> Input::_lastKeyState;
std::map<int, bool> Input::_mouseState;
std::map<int, bool> Input::_lastMouseState;
glm::vec3 Input::_mouseWheelState (glm::vec3 (0.0f));
bool Input::_sdlQuit (false);
glm::vec3 Input::_resizeEvent (glm::vec3 (0.0f));

void Input::UpdateState ()
{
	for (auto const& key : _keyState) {
		_lastKeyState [key.first] = key.second;
	}

	for (auto const& key : _mouseState) {
		_lastMouseState [key.first] = key.second;
	}

	_mouseWheelState = glm::vec3 (0.0f);
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
				_keyState[(int)event.key.keysym.sym] = true;
            	break;
            case SDL_KEYUP:
				_keyState[(int)event.key.keysym.sym] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
				_mouseState [(int)event.button.button] = true;
				break; 
			case SDL_MOUSEBUTTONUP:
				_mouseState [(int)event.button.button] = false;
				break;
			case SDL_MOUSEWHEEL:
				_mouseWheelState.x = event.wheel.x;
				_mouseWheelState.y = event.wheel.y;
				break;
        }
    }
}

bool Input::GetKey (InputKey ch)
{
	return _keyState [(int)ch];
}

bool Input::GetKeyDown (InputKey key)
{
	return _keyState [(int)key] && !_lastKeyState [(int)key];
}

bool Input::GetKeyUp (InputKey key)
{
	return !_keyState [(int)key] && _lastKeyState [(int)key];
}

bool Input::GetMouseButton (unsigned int button)
{
	return _mouseState [button];
}

bool Input::GetMouseButtonDown (unsigned int button)
{
	return _mouseState [button] && !_lastMouseState [button];
}

bool Input::GetMouseButtonUp (unsigned int button)
{
	return !_mouseState [button] && _lastMouseState [button];
}

glm::vec3 Input::GetMouseWheel ()
{
	return _mouseWheelState;
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