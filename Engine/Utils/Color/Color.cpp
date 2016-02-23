#include "Color.h"

Color Color::White(255, 255, 255);
Color Color::Black (0, 0, 0);
Color Color::Red (255, 0, 0);
Color Color::Green (0, 255, 0);
Color Color::Blue (0, 0, 255);

Color::Color () :
	r (),
	g (),
	b (),
	a (255)
{

}

Color::Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
	r (r),
	g (g),
	b (b),
	a (a)
{

}

Color::Color (const Color& other) :
	r (other.r),
	g (other.g),
	b (other.b),
	a (other.a)
{

}

Vector3 Color::ToVector3 ()
{
	return Vector3 (1.0 * r / 255, 1.0 * g / 255, 1.0 * b / 255);	
}