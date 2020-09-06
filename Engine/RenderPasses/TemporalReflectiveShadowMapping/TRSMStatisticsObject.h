#ifndef TRSMSTATISTICSOBJECT_H
#define TRSMSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API TRSMStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(TRSMStatisticsObject)

	FramebufferRenderVolume* trsmIndirectDiffuseMapVolume;

	FramebufferRenderVolume* trsmTemporalFilterMapVolume;
	FramebufferRenderVolume* trsmBlurMapVolume;
};

#endif