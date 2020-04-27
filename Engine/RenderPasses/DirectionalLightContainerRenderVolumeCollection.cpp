#include "DirectionalLightContainerRenderVolumeCollection.h"

DirectionalLightContainerRenderVolumeCollection::DirectionalLightContainerRenderVolumeCollection () :
	_directionalLightsIterator (),
	_volumetricLightVolume (new VolumetricLightVolume ())
{

}

DirectionalLightContainerRenderVolumeCollection::~DirectionalLightContainerRenderVolumeCollection ()
{
	delete _volumetricLightVolume;
}

void DirectionalLightContainerRenderVolumeCollection::Reset (const RenderScene* renderScene)
{
	_directionalLightsIterator = renderScene->begin<RenderDirectionalLightObject*> ();
}

RenderVolumeI* DirectionalLightContainerRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	while (_directionalLightsIterator != renderScene->end<RenderDirectionalLightObject*> ()) {
		auto directionalLightObject = *_directionalLightsIterator;

		if (directionalLightObject->IsActive () == true) {
			break;
		}

		_directionalLightsIterator ++;
	}

	if (_directionalLightsIterator == renderScene->end<RenderDirectionalLightObject*> ()) {
		return nullptr;
	}

	_volumetricLightVolume->SetRenderLightObject (*_directionalLightsIterator);

	_directionalLightsIterator ++;

	return _volumetricLightVolume;
}
