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

Color::Color (const Color& other) :
	r (other.r),
	g (other.g),
	b (other.b),
	a (other.a)
{

}

Color::Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
	r (r),
	g (g),
	b (b),
	a (a)
{

}

Color::Color (const glm::vec3& color) :
	r (color.x * 255),
	g (color.y * 255),
	b (color.z * 255),
	a (255)
{

}

Color::Color (const glm::vec4& color) :
	r (color.x * 255),
	g (color.y * 255),
	b (color.z * 255),
	a (color.a * 255)
{

}

glm::vec3 Color::ToVector3 () const
{
	return glm::vec3 (r / 255.0f, g / 255.0f, b / 255.0f);
}

glm::vec4 Color::ToVector4 () const
{
	return glm::vec4 (r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
