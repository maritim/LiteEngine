#ifndef VCTSTATISTICSOBJECT_H
#define VCTSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API VCTStatisticsObject : public StatisticsObject
{
	FrameBuffer2DVolume* vctIndirectDiffuseMapVolume;
	FrameBuffer2DVolume* vctIndirectSpecularMapVolume;
	FrameBuffer2DVolume* vctAmbientOcclusionMapVolume;
	FrameBuffer2DVolume* vctSubsurfaceScatteringMapVolume;
};

#endif