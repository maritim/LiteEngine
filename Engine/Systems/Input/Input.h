#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <cstdint>
#include <map>

#include "Core/Math/glm/vec3.hpp"
#include "Systems/Input/InputKey.h"

class Input
{
private:
	static std::map<int, bool> _keyState;
	static std::map<int, bool> _lastKeyState;
	static std::map<int, bool> _mouseState;
	static std::map<int, bool> _lastMouseState;
	static glm::vec3 _mouseWheelState;
	static bool _sdlQuit;
	static glm::vec3 _resizeEvent;

public:
	static bool GetKey (InputKey key);
	static bool GetKeyDown (InputKey key);
	static bool GetKeyUp (InputKey key);

	static bool GetMouseButton (unsigned int button);
	static bool GetMouseButtonDown (unsigned int button);
	static bool GetMouseButtonUp (unsigned int button);
	static glm::vec3 GetMouseWheel ();
	static glm::vec3 GetMousePosition ();

	static bool GetQuit ();
	static glm::vec3 GetResizeEvent ();

	static void UpdateState ();
};

#endif