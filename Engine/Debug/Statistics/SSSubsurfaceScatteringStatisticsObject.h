#ifndef SSSUBSURFACESCATTERINGSTATISTICSOBJECT_H
#define SSSUBSURFACESCATTERINGSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API SSSubsurfaceScatteringStatisticsObject : public StatisticsObject
{
	FrameBuffer2DVolume* ssrMapVolume;
};

#endif