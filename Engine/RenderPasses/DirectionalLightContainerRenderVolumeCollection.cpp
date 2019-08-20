#include "DirectionalLightContainerRenderVolumeCollection.h"

DirectionalLightContainerRenderVolumeCollection::DirectionalLightContainerRenderVolumeCollection () :
	_iterations (0),
	_volumetricLightVolume (new VolumetricLightVolume ())
{

}

DirectionalLightContainerRenderVolumeCollection::~DirectionalLightContainerRenderVolumeCollection ()
{
	delete _volumetricLightVolume;
}

void DirectionalLightContainerRenderVolumeCollection::Reset ()
{
	_iterations = 0;
}

RenderVolumeI* DirectionalLightContainerRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene)
{
	if (_iterations > 0) {
		return nullptr;
	}

	auto renderLightObject = renderScene->GetRenderDirectionalLightObject ();

	_volumetricLightVolume->SetRenderLightObject (renderLightObject);

	_iterations ++;

	return _volumetricLightVolume;
}
