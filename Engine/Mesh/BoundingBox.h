#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <limits>

struct BoundingBox
{
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;

	BoundingBox () :
		xmin (std::numeric_limits<float>::max()),
		xmax (std::numeric_limits<float>::min()),
		ymin (std::numeric_limits<float>::max()),
		ymax (std::numeric_limits<float>::min()),
		zmin (std::numeric_limits<float>::max()),
		zmax (std::numeric_limits<float>::min())
	{
	}
};

#endif