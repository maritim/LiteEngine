#include "PointLightContainerRenderVolumeCollection.h"

PointLightContainerRenderVolumeCollection::PointLightContainerRenderVolumeCollection () :
	_pointLightsIterator (),
	_volumetricLightVolume (new VolumetricLightVolume ())
{

}

PointLightContainerRenderVolumeCollection::~PointLightContainerRenderVolumeCollection ()
{
	delete _volumetricLightVolume;
}

void PointLightContainerRenderVolumeCollection::Reset (const RenderScene* renderScene)
{
	_pointLightsIterator = renderScene->begin<RenderPointLightObject*> ();
}

RenderVolumeI* PointLightContainerRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	if (_pointLightsIterator == renderScene->end<RenderPointLightObject*> ()) {
		return nullptr;
	}

	_volumetricLightVolume->SetRenderLightObject (*_pointLightsIterator);

	_pointLightsIterator ++;

	return _volumetricLightVolume;
}
