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

void PointLightContainerRenderVolumeCollection::Reset ()
{
	// _pointLightsIterator = LightsManager::Instance ()->begin<PointLight*> ();
}

RenderVolumeI* PointLightContainerRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	return nullptr;

	// if (_pointLightsIterator == LightsManager::Instance ()->end<PointLight*> ()) {
	// 	return nullptr;
	// }

	// _volumetricLightVolume->SetVolumetricLight (*_pointLightsIterator);

	// _pointLightsIterator ++;

	// return _volumetricLightVolume;
}
