#include "VolumetricLightContainerRenderSubPassI.h"

#include "VolumetricLightVolume.h"

RenderLightObject* VolumetricLightContainerRenderSubPassI::GetRenderLightObject (const RenderVolumeCollection* rvc) const
{
	/*
	 * Get volumetric light volume from render volume collection
	*/

	VolumetricLightVolume* volume = (VolumetricLightVolume*)rvc->GetRenderVolume ("SubpassVolume");

	/*
	 * Get volumetric light from volumetric light volume
	*/

	RenderLightObject* renderLightObject = volume->GetRenderLightObject ();

	return renderLightObject;
}

bool VolumetricLightContainerRenderSubPassI::IsAvailable (const RenderScene*, const Camera*,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Ignore light if it's not active
	*/

	if (!renderLightObject->IsActive ()) {
		return false;
	}

	/*
	 * Use specialization
	*/

	return IsAvailable (renderLightObject);
}