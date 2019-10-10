#ifndef SSRSTATISTICSOBJECT_H
#define SSRSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API SSRStatisticsObject : public StatisticsObject
{
	FrameBuffer2DVolume* ssrPositionMapVolume;
};

#endif