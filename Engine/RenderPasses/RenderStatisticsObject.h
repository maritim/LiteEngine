#ifndef RENDERSTATISTICSOBJECT_H
#define RENDERSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

struct ENGINE_API RenderStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(RenderStatisticsObject)

	std::size_t DrawnVerticesCount;
	std::size_t DrawnPolygonsCount;
	std::size_t DrawnObjectsCount;
};

#endif