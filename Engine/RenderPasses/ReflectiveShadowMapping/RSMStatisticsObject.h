#ifndef RSMSTATISTICSOBJECT_H
#define RSMSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API RSMStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(RSMStatisticsObject)

	FramebufferRenderVolume* rsmVolume;

	FramebufferRenderVolume* rsmIndirectDiffuseMapVolume;
	FramebufferRenderVolume* rsmIndirectSpecularMapVolume;
	FramebufferRenderVolume* rsmSubsurfaceScatteringMapVolume;
	FramebufferRenderVolume* rsmAmbientOcclusionMapVolume;

	FramebufferRenderVolume* rsmInterpolatedIndirectDiffuseMapVolume;
};

#endif