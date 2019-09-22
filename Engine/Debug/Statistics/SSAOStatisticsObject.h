#ifndef SSAOSTATISTICSOBJECT_H
#define SSAOSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API SSAOStatisticsObject : public StatisticsObject
{
	FrameBuffer2DVolume* ssaoMapVolume;
	unsigned int noiseMapID;
};

#endif