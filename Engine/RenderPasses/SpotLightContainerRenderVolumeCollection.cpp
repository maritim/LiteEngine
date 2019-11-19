#include "SpotLightContainerRenderVolumeCollection.h"

SpotLightContainerRenderVolumeCollection::SpotLightContainerRenderVolumeCollection () :
	_spotLightsIterator (),
	_volumetricLightVolume (new VolumetricLightVolume ())
{

}

SpotLightContainerRenderVolumeCollection::~SpotLightContainerRenderVolumeCollection ()
{
	delete _volumetricLightVolume;
}

void SpotLightContainerRenderVolumeCollection::Reset (const RenderScene* renderScene)
{
	_spotLightsIterator = renderScene->begin<RenderSpotLightObject*> ();
}

RenderVolumeI* SpotLightContainerRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	while (_spotLightsIterator != renderScene->end<RenderSpotLightObject*> ()) {
		auto spotLightObject = *_spotLightsIterator;

		if (spotLightObject->IsActive () == true) {
			break;
		}

		_spotLightsIterator ++;
	}

	if (_spotLightsIterator == renderScene->end<RenderSpotLightObject*> ()) {
		return nullptr;
	}

	_volumetricLightVolume->SetRenderLightObject (*_spotLightsIterator);

	_spotLightsIterator ++;

	return _volumetricLightVolume;
}
