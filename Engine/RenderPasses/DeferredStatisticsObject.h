#ifndef DEFERREDSTATISTICSOBJECT_H
#define DEFERREDSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API DeferredStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(DeferredStatisticsObject)

	FramebufferRenderVolume* geometryBuffer;
	FramebufferRenderVolume* translucencyGeometryBuffer;
};

#endif