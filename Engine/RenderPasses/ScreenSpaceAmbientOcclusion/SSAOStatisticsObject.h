#ifndef SSAOSTATISTICSOBJECT_H
#define SSAOSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"
#include "RenderPasses/TextureRenderVolume.h"

struct ENGINE_API SSAOStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(SSAOStatisticsObject)

	FramebufferRenderVolume* ssaoMapVolume;
	TextureRenderVolume* ssaoNoiseMapVolume;
};

#endif