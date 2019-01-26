#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Singleton/Singleton.h"

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <string>

class Window
{
private:
	static std::size_t _width;
	static std::size_t _height;
	static bool _fullscreen;
	static std::string _title;

	static SDL_Window* _window;
	static SDL_GLContext _glContext;

public:
	static bool Init ();

	static void SwapBuffers ();
	static void Resize (const glm::ivec2&);

	static std::size_t GetWidth ();
	static std::size_t GetHeight ();

	static std::string GetTitle ();

	static void Clear ();
	static void Close ();
private:
	static bool InitSettings ();

	static void UpdateCamera ();
};

#endif