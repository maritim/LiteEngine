#ifndef LPVSTATISTICSOBJECT_H
#define LPVSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API LPVStatisticsObject : public StatisticsObject
{
	FrameBuffer2DVolume* lpvIndirectDiffuseMapVolume;
	FrameBuffer2DVolume* lpvIndirectSpecularMapVolume;
};

#endif