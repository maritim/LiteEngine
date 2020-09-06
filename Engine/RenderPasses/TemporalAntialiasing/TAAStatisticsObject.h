#ifndef TAASTATISTICSOBJECT_H
#define TAASTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API TAAStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(TAAStatisticsObject)

	FramebufferRenderVolume* taaMapVolume;
};

#endif