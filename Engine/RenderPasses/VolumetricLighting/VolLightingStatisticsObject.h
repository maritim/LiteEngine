#ifndef VOLLIGHTINGSTATISTICSOBJECT_H
#define VOLLIGHTINGSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API VolLightingStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(VolLightingStatisticsObject)

	FramebufferRenderVolume* volumetricLightMapVolume;
};

#endif