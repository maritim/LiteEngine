#ifndef LIGHTSHAFTSSTATISTICSOBJECT_H
#define LIGHTSHAFTSSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API LightShaftsStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(LightShaftsStatisticsObject)

	FramebufferRenderVolume* lightShaftsMapVolume;
};

#endif