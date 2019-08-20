#include "VolumetricLightVolume.h"

VolumetricLightVolume::VolumetricLightVolume () :
	_renderLightObject (nullptr)
{

}

void VolumetricLightVolume::SetRenderLightObject (RenderLightObject* renderLightObject)
{
	_renderLightObject = renderLightObject;
}

RenderLightObject* VolumetricLightVolume::GetRenderLightObject () const
{
	return _renderLightObject;
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

void VolumetricLightVolume::Clear ()
{
	/*
	 * Nothing
	*/
}