#ifndef SSRSTATISTICSOBJECT_H
#define SSRSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API SSRStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(SSRStatisticsObject)

	FramebufferRenderVolume* ssrPositionMapVolume;
	FramebufferRenderVolume* ssrMapVolume;
};

#endif