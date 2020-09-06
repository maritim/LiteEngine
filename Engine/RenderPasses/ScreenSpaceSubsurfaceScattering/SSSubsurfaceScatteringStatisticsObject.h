#ifndef SSSUBSURFACESCATTERINGSTATISTICSOBJECT_H
#define SSSUBSURFACESCATTERINGSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API SSSubsurfaceScatteringStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(SSSubsurfaceScatteringStatisticsObject)

	FramebufferRenderVolume* ssrMapVolume;
};

#endif