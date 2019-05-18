#ifndef DIRECTIONALLIGHTRENDERCONTAINERVOLUMECOLLECTION_H
#define DIRECTIONALLIGHTRENDERCONTAINERVOLUMECOLLECTION_H

#include "RenderPasses/Container/ContainerRenderVolumeCollectionI.h"

#include <vector>

#include "RenderPasses/VolumetricLightVolume.h"
#include "Lighting/DirectionalLight.h"

class ENGINE_API DirectionalLightContainerRenderVolumeCollection : public ContainerRenderVolumeCollectionI
{
protected:
	std::vector<DirectionalLight*>::iterator _directionalLightsIterator;
	VolumetricLightVolume* _volumetricLightVolume;

public:
	DirectionalLightContainerRenderVolumeCollection ();
	~DirectionalLightContainerRenderVolumeCollection ();

	void Reset ();
	RenderVolumeI* GetNextVolume ();
};

#endif