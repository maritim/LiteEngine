#ifndef SPOTLIGHTRENDERCONTAINERVOLUMECOLLECTION_H
#define SPOTLIGHTRENDERCONTAINERVOLUMECOLLECTION_H

#include "RenderPasses/Container/ContainerRenderVolumeCollectionI.h"

#include <vector>

#include "RenderPasses/VolumetricLightVolume.h"
#include "Renderer/RenderSpotLightObject.h"

class ENGINE_API SpotLightContainerRenderVolumeCollection : public ContainerRenderVolumeCollectionI
{
protected:
	std::set<RenderSpotLightObject*>::iterator _spotLightsIterator;
	VolumetricLightVolume* _volumetricLightVolume;

public:
	SpotLightContainerRenderVolumeCollection ();
	~SpotLightContainerRenderVolumeCollection ();

	void Reset (const RenderScene*);
	RenderVolumeI* GetNextVolume (const RenderScene*, const RenderSettings&);
};

#endif