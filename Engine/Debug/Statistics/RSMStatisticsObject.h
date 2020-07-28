#ifndef RSMSTATISTICSOBJECT_H
#define RSMSTATISTICSOBJECT_H

#include "StatisticsObject.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

struct ENGINE_API RSMStatisticsObject : public StatisticsObject
{
	unsigned int rsmPosMapID;
	unsigned int rsmNormalMapID;
	unsigned int rsmFluxMapID;
	unsigned int rsmDepthMapID;

	FrameBuffer2DVolume* rsmIndirectDiffuseMapVolume;
	FrameBuffer2DVolume* rsmIndirectSpecularMapVolume;
};

#endif