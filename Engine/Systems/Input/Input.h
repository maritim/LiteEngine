#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glm/vec2.hpp>
#include <cstdint>
#include <string>
#include <map>

#include "Systems/Input/InputKey.h"

class Input
{
private:

	/*
	 * Keyboard state
	*/

	static std::map<int, bool> _keyState;
	static std::map<int, bool> _lastKeyState;

	/*
	 * Mouse state
	*/

	static std::map<int, bool> _mouseState;
	static std::map<int, bool> _lastMouseState;
	static glm::ivec2 _mouseWheelState;

	/*
	 * Joysticks/Game controllers state
	*/

	static std::map<int, std::map<int, bool> > _joystickButtonState;
	static std::map<int, std::map<int, bool> > _lastJoystickButtonState;
	static std::map<int, std::map<int, std::int16_t> > _joystickAxisState;

	/*
	 * Text
	*/

	static std::string _imeTextComposition;

	/*
	 * Miscellaneous
	*/

	static bool _sdlQuit;
	static glm::ivec2 _resizeEvent;

public:
	static bool GetKey (InputKey key);
	static bool GetKeyDown (InputKey key);
	static bool GetKeyUp (InputKey key);

	static bool GetMouseButton (std::uint8_t button);
	static bool GetMouseButtonDown (std::uint8_t button);
	static bool GetMouseButtonUp (std::uint8_t button);
	static glm::ivec2 GetMouseWheel ();
	static glm::ivec2 GetMousePosition ();

	static bool GetJoystickButton (std::uint8_t button, std::uint8_t joystick = 0);
	static bool GetJoystickButtonDown (std::uint8_t button, std::uint8_t joystick = 0);
	static bool GetJoystickButtonUp (std::uint8_t button, std::uint8_t joystick = 0);
	static glm::ivec2 GetJoystickAxis (std::uint8_t axis = 0, std::uint8_t joystick = 0);

	static std::string GetIMETextComposition ();

	static bool GetQuit ();
	static glm::ivec2 GetResizeEvent ();

	static void Init ();
	static void UpdateState ();
private:
	static void InitJoysticks ();
};

#endif