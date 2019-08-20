#ifndef RENDERAMBIENTLIGHTOBJECT_H
#define RENDERAMBIENTLIGHTOBJECT_H

#include "Utils/Color/Color.h"

struct RenderAmbientLightObject
{
	Color color;
	float intensity;
	bool isActive;

	RenderAmbientLightObject () :
		color (Color::Black),
		intensity (1.0f),
		isActive (true)
	{

	}
};

#endif