#include "Window.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Camera/Camera.h"

std::size_t Window::_width (0);
std::size_t Window::_height (0);
std::string Window::_title ("");

SDL_Window* Window::_window (nullptr);
SDL_GLContext Window::_glContext;

bool Window::Init (std::size_t width, std::size_t height, std::string title)
{
	_width = width; 
	_height = height; 
	_title = title;

	/*
	 * Create Window with SDL so MUST include SDL_WINDOW_OPENGL to use OpenGL
	*/
	
	_window = SDL_CreateWindow (_title.c_str (), 0, 0, _width, _height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);

	if (_window == nullptr) {
		Console::LogError ("Window \"" + title + "\" could not be initialized");

		return false;
	}

	/*
	 * Create an OpenGL context associated with the window.
	*/

	_glContext = SDL_GL_CreateContext(_window);

	GL::Viewport (0, 0, _width, _height);

	Window::UpdateCamera ();

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

	GL::Viewport (0, 0, _width, _height);

	Window::UpdateCamera ();
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

/*
 * TODO: Rethink this
*/

void Window::UpdateCamera ()
{
	Camera::Main ()->SetAspect (1.0f * _width / _height);
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