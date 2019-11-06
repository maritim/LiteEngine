#ifndef POINTLIGHTRENDERCONTAINERVOLUMECOLLECTION_H
#define POINTLIGHTRENDERCONTAINERVOLUMECOLLECTION_H

#include "RenderPasses/Container/ContainerRenderVolumeCollectionI.h"

#include <vector>

#include "RenderPasses/VolumetricLightVolume.h"
#include "Renderer/RenderPointLightObject.h"

class ENGINE_API PointLightContainerRenderVolumeCollection : public ContainerRenderVolumeCollectionI
{
protected:
	std::set<RenderPointLightObject*>::iterator _pointLightsIterator;
	VolumetricLightVolume* _volumetricLightVolume;

public:
	PointLightContainerRenderVolumeCollection ();
	~PointLightContainerRenderVolumeCollection ();

	void Reset (const RenderScene*);
	RenderVolumeI* GetNextVolume (const RenderScene*, const RenderSettings&);
};

#endif