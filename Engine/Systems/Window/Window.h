#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Singleton/Singleton.h"

#include <SDL2/SDL.h>
#include <string>

#include "Core/Math/glm/vec3.hpp"

class Window
{
private:
	static std::size_t _width;
	static std::size_t _height;
	static std::string _title;

	static SDL_Window* _window;
	static SDL_GLContext _glContext;

public:
	static bool Init (std::size_t width, std::size_t height, std::string name);

	static void SwapBuffers ();
	static void Resize (const glm::vec3&);

	static std::size_t GetWidth ();
	static std::size_t GetHeight ();
	static std::string GetTitle ();

	static void Clear ();
	static void Close ();
private:
	static void UpdateCamera ();
};

#endif