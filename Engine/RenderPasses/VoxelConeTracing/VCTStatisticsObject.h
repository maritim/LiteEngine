#ifndef VCTSTATISTICSOBJECT_H
#define VCTSTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API VCTStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(VCTStatisticsObject)

	FramebufferRenderVolume* vctIndirectDiffuseMapVolume;
	FramebufferRenderVolume* vctIndirectSpecularMapVolume;
	FramebufferRenderVolume* vctAmbientOcclusionMapVolume;
	FramebufferRenderVolume* vctSubsurfaceScatteringMapVolume;
	FramebufferRenderVolume* vctShadowVolume;
};

#endif