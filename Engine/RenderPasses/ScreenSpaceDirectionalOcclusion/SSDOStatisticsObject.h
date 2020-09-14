#ifndef SSDOSTATISTICSOBJECT_H
#define SSDOSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API SSDOStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(SSDOStatisticsObject)

	FramebufferRenderVolume* ssdoMapVolume;
	FramebufferRenderVolume* ssdoTemporalFilterMapVolume;

	FramebufferRenderVolume* ssdoInterpolatedMapVolume;

	FramebufferRenderVolume* ssdoShadowVolume;
};

#endif