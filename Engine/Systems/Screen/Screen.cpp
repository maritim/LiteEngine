#include "Screen.h"

#include "Systems/Window/Window.h"

Screen::Screen ()
{

}

Screen::~Screen ()
{
	
}

std::size_t Screen::GetWidth ()
{
	return Window::GetWidth ();
}

std::size_t Screen::GetHeight ()
{
	return Window::GetHeight ();
}

glm::uvec2 Screen::GetSize ()
{
	return glm::uvec2 (Window::GetWidth (), Window::GetHeight ());
}

void Screen::SetWidth (std::size_t width)
{
	/* 
	 * TODO: Update this. Send signal to Window.
	*/

//	_screenWidth = width;
}

void Screen::SetHeight (std::size_t height)
{
//	_screenHeight = height;
}