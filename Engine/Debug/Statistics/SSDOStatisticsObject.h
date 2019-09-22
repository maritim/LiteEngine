#ifndef SSDOSTATISTICSOBJECT_H
#define SSDOSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API SSDOStatisticsObject : public StatisticsObject
{
	FrameBuffer2DVolume* ssdoMapVolume;
};

#endif