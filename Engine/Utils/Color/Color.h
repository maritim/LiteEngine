#ifndef COLOR_H
#define COLOR_H

#include "Core/Interfaces/Object.h"

#include "Core/Math/glm/vec3.hpp"

class Color : public Object
{
public:
	static Color White;
	static Color Black;
	static Color Red;
	static Color Green;
	static Color Blue;

	unsigned char r, g, b, a;

public:
	Color ();
	Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	Color (const Color& other);

	glm::vec3 ToVector3 ();
};

#endif