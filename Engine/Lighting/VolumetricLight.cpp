#include "VolumetricLight.h"

VolumetricLight::VolumetricLight () :
	_volume (nullptr),
	_lightRenderer (nullptr)
{
	_lightRenderer = new LightRenderer (this);
}

VolumetricLight::~VolumetricLight ()
{
	delete _volume;
}

void VolumetricLight::SetVolume (Model* volume)
{
	_volume = volume;

	_lightRenderer->Attach (volume);
}

Model* VolumetricLight::GetVolume ()
{
	return _volume;
}

LightRenderer* VolumetricLight::GetLightRenderer ()
{
	return _lightRenderer;
}