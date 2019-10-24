#ifndef RENDERSTATISTICSOBJECT_H
#define RENDERSTATISTICSOBJECT_H

#include "StatisticsObject.h"

struct ENGINE_API RenderStatisticsObject : public StatisticsObject
{
	std::size_t DrawnVerticesCount;
	std::size_t DrawnPolygonsCount;
	std::size_t DrawnObjectsCount;
};

#endif