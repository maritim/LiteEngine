#ifndef LPVSTATISTICSOBJECT_H
#define LPVSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API LPVStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(LPVStatisticsObject)

	FramebufferRenderVolume* lpvIndirectDiffuseMapVolume;
	FramebufferRenderVolume* lpvIndirectSpecularMapVolume;
	FramebufferRenderVolume* lpvSubsurfaceScatteringMapVolume;
	FramebufferRenderVolume* lpvAmbientOcclusionMapVolume;
};

#endif