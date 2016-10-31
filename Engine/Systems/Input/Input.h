#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <cstdint>
#include <map>

#include "Core/Math/glm/vec3.hpp"
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
	static glm::vec3 _mouseWheelState;

	/*
	 * Joysticks/Game controllers state
	*/

	static std::map<int, std::map<int, bool> > _joystickButtonState;
	static std::map<int, std::map<int, bool> > _lastJoystickButtonState;
	static std::map<int, std::map<int, std::int16_t> > _joystickAxisState;

	/*
	 * Miscellaneous
	*/

	static bool _sdlQuit;
	static glm::vec3 _resizeEvent;

public:
	static bool GetKey (InputKey key);
	static bool GetKeyDown (InputKey key);
	static bool GetKeyUp (InputKey key);

	static bool GetMouseButton (std::uint8_t button);
	static bool GetMouseButtonDown (std::uint8_t button);
	static bool GetMouseButtonUp (std::uint8_t button);
	static glm::vec3 GetMouseWheel ();
	static glm::vec3 GetMousePosition ();

	static bool GetJoystickButton (std::uint8_t button, std::uint8_t joystick = 0);
	static bool GetJoystickButtonDown (std::uint8_t button, std::uint8_t joystick = 0);
	static bool GetJoystickButtonUp (std::uint8_t button, std::uint8_t joystick = 0);
	static glm::vec3 GetJoystickAxis (std::uint8_t axis = 0, std::uint8_t joystick = 0);

	static bool GetQuit ();
	static glm::vec3 GetResizeEvent ();

	static void Init ();
	static void UpdateState ();
private:
	static void InitJoysticks ();
};

#endif