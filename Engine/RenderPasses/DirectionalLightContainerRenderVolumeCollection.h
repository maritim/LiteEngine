#ifndef DIRECTIONALLIGHTRENDERCONTAINERVOLUMECOLLECTION_H
#define DIRECTIONALLIGHTRENDERCONTAINERVOLUMECOLLECTION_H

#include "RenderPasses/Container/ContainerRenderVolumeCollectionI.h"

#include <vector>

#include "RenderPasses/VolumetricLightVolume.h"
#include "Renderer/RenderDirectionalLightObject.h"

class ENGINE_API DirectionalLightContainerRenderVolumeCollection : public ContainerRenderVolumeCollectionI
{
protected:
	std::set<RenderDirectionalLightObject*>::iterator _directionalLightsIterator;
	VolumetricLightVolume* _volumetricLightVolume;

public:
	DirectionalLightContainerRenderVolumeCollection ();
	~DirectionalLightContainerRenderVolumeCollection ();

	void Reset (const RenderScene*);
	RenderVolumeI* GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings);
};

#endif