#ifndef DIRECTIONALLIGHTRENDERCONTAINERVOLUMECOLLECTION_H
#define DIRECTIONALLIGHTRENDERCONTAINERVOLUMECOLLECTION_H

#include "RenderPasses/Container/ContainerRenderVolumeCollectionI.h"

#include <vector>

#include "RenderPasses/VolumetricLightVolume.h"

class ENGINE_API DirectionalLightContainerRenderVolumeCollection : public ContainerRenderVolumeCollectionI
{
protected:
	std::size_t _iterations;
	VolumetricLightVolume* _volumetricLightVolume;

public:
	DirectionalLightContainerRenderVolumeCollection ();
	~DirectionalLightContainerRenderVolumeCollection ();

	void Reset ();
	RenderVolumeI* GetNextVolume (const RenderScene* renderScene);
};

#endif