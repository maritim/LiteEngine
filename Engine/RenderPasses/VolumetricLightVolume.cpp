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

const std::vector<PipelineAttribute>& VolumetricLightVolume::GetCustomAttributes () const
{
	/*
	 * Nothing to do here
	*/

	return _attributes;
}
