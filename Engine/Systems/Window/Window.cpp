#include "Window.h"

#include "Systems/Settings/SettingsManager.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Camera/Camera.h"

std::size_t Window::_width (0);
std::size_t Window::_height (0);
bool Window::_fullscreen (true);
std::string Window::_title ("");

SDL_Window* Window::_window (nullptr);
SDL_GLContext Window::_glContext;

bool Window::Init ()
{
	/*
	 * Initialize values based on settings
	*/

	if (InitSettings () == false) {
		return false;
	}

	/*
	 * Create Window with SDL so MUST include SDL_WINDOW_OPENGL to use OpenGL
	*/

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	
	std::size_t windowFlags = SDL_WINDOW_OPENGL | (_fullscreen ? SDL_WINDOW_FULLSCREEN : 0) |
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;

	_window = SDL_CreateWindow (_title.c_str (), 0, 0, _width, _height, windowFlags);

	if (_window == nullptr) {
		Console::LogError ("Window \"" + _title + "\" could not be initialized");
		return false;
	}

	/*
	 * Create an OpenGL context associated with the window.
	*/

	_glContext = SDL_GL_CreateContext(_window);

	return true;
}

void Window::SwapBuffers ()
{
	SDL_GL_SwapWindow(_window);
}

void Window::Resize (const glm::ivec2& dimensions)
{
	_width = (std::size_t)dimensions.x;
	_height = (std::size_t)dimensions.y;

	SDL_SetWindowSize (_window, _width, _height);
}

void Window::SetMousePosition (const glm::ivec2& position)
{
	SDL_WarpMouseInWindow (_window, position.x, position.y);
}

void Window::Clear ()
{

}

void Window::Close ()
{
	/* 
	 * Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	*/

	SDL_GL_DeleteContext(_glContext);

	SDL_DestroyWindow (_window);
}

std::size_t Window::GetWidth ()
{
	return _width;
}

std::size_t Window::GetHeight ()
{
	return _height;
}

std::string Window::GetTitle ()
{
	return _title;
}

bool Window::InitSettings ()
{
	/*
	 * Initialize window resolution
	*/

	glm::vec2 size = SettingsManager::Instance ()->GetValue<glm::vec2> ("Window", "resolution", glm::vec2 (0, 0));

	if (size.x == 0 && size.y == 0) {
		Console::LogError ("Window resolution not specified. Could not proceed further");
		return false;
	}

	_width = (std::size_t) size.x;
	_height = (std::size_t) size.y;

	/*
	 * Initialize window mode
	*/

	_fullscreen = SettingsManager::Instance ()->GetValue<bool> ("Window", "fullscreen", true);

	/*
	 * Initialize window title
	*/

	_title = SettingsManager::Instance ()->GetValue<std::string> ("Window", "title", "");

	return true;
}
