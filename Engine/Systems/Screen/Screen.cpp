#include "Screen.h"

Screen::Screen () :
	_screenWidth (0),
	_screenHeight (0)
{

}

Screen::~Screen ()
{
	
}

std::size_t Screen::GetWidth ()
{
	return _screenWidth;
}

std::size_t Screen::GetHeight ()
{
	return _screenHeight;
}

glm::uvec2 Screen::GetSize ()
{
	return glm::uvec2 (_screenWidth, _screenHeight);
}

void Screen::SetWidth (std::size_t width)
{
	_screenWidth = width;
}

void Screen::SetHeight (std::size_t height)
{
	_screenHeight = height;
}