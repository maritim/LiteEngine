#include "VolumetricLightContainerRenderSubPassI.h"

#include "VolumetricLightVolume.h"

VolumetricLight* VolumetricLightContainerRenderSubPassI::GetVolumetricLight (const RenderVolumeCollection* rvc) const
{
	/*
	 * Get volumetric light volume from render volume collection
	*/

	VolumetricLightVolume* volume = (VolumetricLightVolume*)rvc->GetRenderVolume ("SubpassVolume");

	/*
	 * Get volumetric light from volumetric light volume
	*/

	VolumetricLight* volumetricLight = volume->GetVolumetricLight ();

	return volumetricLight;
}

bool VolumetricLightContainerRenderSubPassI::IsAvailable (const Scene*, const Camera*, const RenderVolumeCollection* rvc) const
{
	/*
	 * Get volumetric light from render volume collection
	*/

	VolumetricLight* volumetricLight = GetVolumetricLight (rvc);

	/*
	 * Ignore light if it's not active
	*/

	if (!volumetricLight->IsActive ()) {
		return false;
	}

	/*
	 * Use specialization
	*/

	return IsAvailable (volumetricLight);
}