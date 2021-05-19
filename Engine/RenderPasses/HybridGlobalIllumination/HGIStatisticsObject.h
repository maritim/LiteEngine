#ifndef HGISTATISTICSOBJECT_H
#define HGISTATISTICSOBJECT_H

#include "Debug/Statistics/StatisticsObject.h"

#include "RenderPasses/FramebufferRenderVolume.h"

struct ENGINE_API HGIStatisticsObject : public StatisticsObject
{
	DECLARE_STATISTICS_OBJECT(HGIStatisticsObject)

	FramebufferRenderVolume* hgiDirectMapVolume;
	FramebufferRenderVolume* hgiRSMIndirectDiffuseMapVolume;
	FramebufferRenderVolume* hgiSSDOIndirectDiffuseMapVolume;

	FramebufferRenderVolume* hgiIndirectDiffuseMapVolume;
	FramebufferRenderVolume* hgiTemporalFilterMapVolume;

	FramebufferRenderVolume* hgiSSRPositionMapVolume;
	FramebufferRenderVolume* hgiIndirectSpecularMapVolume;

	FramebufferRenderVolume* hgiRSMAmbientOcclusionMapVolume;
	FramebufferRenderVolume* hgiAmbientOcclusionMapVolume;
	FramebufferRenderVolume* hgiAOBlurMapVolume;
	FramebufferRenderVolume* hgiAOTemporalFilterMapVolume;

	// FramebufferRenderVolume* hgiIndirectSpecularMapVolume;
	// FramebufferRenderVolume* hgiSubsurfaceScatteringMapVolume;
	// FramebufferRenderVolume* hgiAmbientOcclusionMapVolume;

	FramebufferRenderVolume* hgiRSMInterpolatedIndirectDiffuseMapVolume;
	FramebufferRenderVolume* hgiSSDOInterpolatedIndirectDiffuseMapVolume;
};

#endif