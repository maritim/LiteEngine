#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <limits>

struct BoundingBox
{
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;

	BoundingBox () :
		xmin (std::numeric_limits<float>::infinity()),
		xmax (-std::numeric_limits<float>::infinity()),
		ymin (std::numeric_limits<float>::infinity()),
		ymax (-std::numeric_limits<float>::infinity()),
		zmin (std::numeric_limits<float>::infinity()),
		zmax (-std::numeric_limits<float>::infinity())
	{
	}
};

#endif