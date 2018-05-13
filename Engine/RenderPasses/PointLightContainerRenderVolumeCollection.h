#ifndef POINTLIGHTRENDERCONTAINERVOLUMECOLLECTION_H
#define POINTLIGHTRENDERCONTAINERVOLUMECOLLECTION_H

#include "RenderPasses/Container/ContainerRenderVolumeCollectionI.h"

#include <vector>

#include "RenderPasses/VolumetricLightVolume.h"
#include "Lighting/PointLight.h"

class PointLightContainerRenderVolumeCollection : public ContainerRenderVolumeCollectionI
{
protected:
	std::vector<PointLight*>::iterator _pointLightsIterator;
	VolumetricLightVolume* _volumetricLightVolume;

public:
	PointLightContainerRenderVolumeCollection ();
	~PointLightContainerRenderVolumeCollection ();

	void Reset ();
	RenderVolumeI* GetNextVolume ();
};

#endif