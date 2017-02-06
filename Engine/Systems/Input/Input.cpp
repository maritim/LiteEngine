#include "Input.h"

std::map<int, bool> Input::_keyState;
std::map<int, bool> Input::_lastKeyState;

std::map<int, bool> Input::_mouseState;
std::map<int, bool> Input::_lastMouseState;
glm::ivec2 Input::_mouseWheelState (glm::ivec2 (0));

std::map<int, std::map<int, bool> > Input::Input::_joystickButtonState;
std::map<int, std::map<int, bool> > Input::_lastJoystickButtonState;
std::map<int, std::map<int, std::int16_t> > Input::_joystickAxisState;

bool Input::_sdlQuit (false);
glm::ivec2 Input::_resizeEvent (glm::ivec2 (0));

void Input::Init ()
{
	InitJoysticks ();
}

void Input::InitJoysticks ()
{
	SDL_JoystickEventState(SDL_ENABLE);

	for (int joystickIndex=0;joystickIndex<SDL_NumJoysticks ();joystickIndex ++) {
		SDL_JoystickOpen (joystickIndex);
	}
}

void Input::UpdateState ()
{
	/*
	 * Update/Clear last input state
	*/

	for (auto const& key : _keyState) {
		_lastKeyState [key.first] = key.second;
	}

	for (auto const& key : _mouseState) {
		_lastMouseState [key.first] = key.second;
	}

	for (auto const& joystick : _joystickButtonState) {
		for (auto const& key : joystick.second) {
			_lastJoystickButtonState [joystick.first] [key.first] = key.second;
		}		
	}

	_mouseWheelState = glm::ivec2 (0);
	_resizeEvent = glm::ivec2 (0);

	/*
	 * Update input state
	*/

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
			case SDL_JOYBUTTONDOWN: {
					int joystick = (int)event.jbutton.which;
					int button = (int)event.jbutton.button;
					_joystickButtonState [joystick][button] = true;
				}
				break;
			case SDL_JOYBUTTONUP: {
					int joystick = (int)event.jbutton.which;
					int button = (int)event.jbutton.button;
					_joystickButtonState [joystick][button] = false;
				}
				break;
			case SDL_JOYAXISMOTION: {
					int joystick = (int)event.jaxis.which;
					int axis = (int)event.jaxis.axis;
					_joystickAxisState [joystick][axis] = event.jaxis.value;
				}
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

bool Input::GetMouseButton (std::uint8_t button)
{
	return _mouseState [button];
}

bool Input::GetMouseButtonDown (std::uint8_t button)
{
	return _mouseState [button] && !_lastMouseState [button];
}

bool Input::GetMouseButtonUp (std::uint8_t button)
{
	return !_mouseState [button] && _lastMouseState [button];
}

glm::ivec2 Input::GetMouseWheel ()
{
	return _mouseWheelState;
}

glm::ivec2 Input::GetMousePosition ()
{
	int x = 0, y = 0;

	SDL_GetMouseState(&x, &y);

	glm::ivec2 position;
	position.x = x;
	position.y = y;

	return position;
}

bool Input::GetJoystickButton (std::uint8_t button, std::uint8_t joystick)
{
	if (_joystickButtonState.find (joystick) == _joystickButtonState.end ()) {
		return false;
	}

	return _joystickButtonState [joystick][button];
}

bool Input::GetJoystickButtonDown (std::uint8_t button, std::uint8_t joystick)
{
	if (_joystickButtonState.find (joystick) == _joystickButtonState.end () ||
			_lastJoystickButtonState.find (joystick) == _lastJoystickButtonState.end ()) {
		return false;
	}

	return _joystickButtonState [joystick][button] && !_lastJoystickButtonState [joystick][button];
}

bool Input::GetJoystickButtonUp (std::uint8_t button, std::uint8_t joystick)
{
	if (_joystickButtonState.find (joystick) == _joystickButtonState.end () ||
			_lastJoystickButtonState.find (joystick) == _lastJoystickButtonState.end ()) {
		return false;
	}

	return !_joystickButtonState [joystick][button] && _lastJoystickButtonState [joystick][button];
}

glm::ivec2 Input::GetJoystickAxis (std::uint8_t axis, std::uint8_t joystick)
{
	if (_joystickAxisState.find (joystick) == _joystickAxisState.end ()) {
		return glm::ivec2 (0);
	}

	std::uint8_t realAxisX = axis * 2;
	std::uint8_t realAxisY = axis * 2 + 1;

	glm::ivec2 axisValue;
	axisValue.x = _joystickAxisState [joystick][realAxisX];
	axisValue.y = _joystickAxisState [joystick][realAxisY];

	return axisValue;
}

bool Input::GetQuit ()
{
	return _sdlQuit;
}

glm::ivec2 Input::GetResizeEvent ()
{
	return _resizeEvent;
}