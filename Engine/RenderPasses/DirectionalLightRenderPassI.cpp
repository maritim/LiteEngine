#include "DirectionalLightRenderPassI.h"

#include "Lighting/LightsManager.h"

DirectionalLightRenderPassI::DirectionalLightRenderPassI () :
	_volume (new VolumetricLightVolume ()),
	_lightsIterator (0)
{

}

DirectionalLightRenderPassI::~DirectionalLightRenderPassI ()
{
	delete _volume;
}

RenderVolumeI* DirectionalLightRenderPassI::NextVolume ()
{
	if (_lightsIterator >= LightsManager::Instance ()->GetDirectionalLightsCount ()) {
		_lightsIterator = 0;

		return nullptr;
	}

	VolumetricLight* volumetricLight = LightsManager::Instance ()->GetDirectionalLight (_lightsIterator);
	_volume->SetVolumetricLight (volumetricLight);

	_lightsIterator ++;

	return _volume;
}
