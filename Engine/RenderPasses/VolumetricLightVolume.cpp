#include "VolumetricLightVolume.h"

VolumetricLightVolume::VolumetricLightVolume () :
	_volumetricLight (nullptr)
{

}

void VolumetricLightVolume::SetVolumetricLight (VolumetricLight* volumetricLight)
{
	_volumetricLight = volumetricLight;
}

VolumetricLight* VolumetricLightVolume::GetVolumetricLight () const
{
	return _volumetricLight;
}

void VolumetricLightVolume::BindForReading ()
{
	/*
	 * Nothing to do here
	*/
}

void VolumetricLightVolume::BindForWriting ()
{
	/*
	 * Nothing to do here
	*/
}

std::vector<PipelineAttribute> VolumetricLightVolume::GetCustomAttributes () const
{
	/*
	 * Nothing to do here
	*/

	return std::vector<PipelineAttribute> ();
}