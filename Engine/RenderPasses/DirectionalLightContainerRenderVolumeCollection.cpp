#include "DirectionalLightContainerRenderVolumeCollection.h"

#include "Lighting/LightsManager.h"

DirectionalLightContainerRenderVolumeCollection::DirectionalLightContainerRenderVolumeCollection () :
	_directionalLightsIterator (),
	_volumetricLightVolume (new VolumetricLightVolume ())
{

}

DirectionalLightContainerRenderVolumeCollection::~DirectionalLightContainerRenderVolumeCollection ()
{
	delete _volumetricLightVolume;
}

void DirectionalLightContainerRenderVolumeCollection::Reset ()
{
	_directionalLightsIterator = LightsManager::Instance ()->begin<DirectionalLight*> ();
}

RenderVolumeI* DirectionalLightContainerRenderVolumeCollection::GetNextVolume ()
{
	_directionalLightsIterator ++;

	if (_directionalLightsIterator == LightsManager::Instance ()->end<DirectionalLight*> ()) {
		return nullptr;
	}

	_volumetricLightVolume->SetVolumetricLight (*_directionalLightsIterator);

	return _volumetricLightVolume;
}
