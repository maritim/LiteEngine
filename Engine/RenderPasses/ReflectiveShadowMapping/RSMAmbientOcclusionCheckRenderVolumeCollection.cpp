#include "RSMAmbientOcclusionCheckRenderVolumeCollection.h"

RSMAmbientOcclusionCheckRenderVolumeCollection::RSMAmbientOcclusionCheckRenderVolumeCollection () :
	IterateOverRenderVolumeCollection (1)
{

}

RenderVolumeI* RSMAmbientOcclusionCheckRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	if (settings.rsm_ao_enabled == false) {
		return nullptr;
	}

	return IterateOverRenderVolumeCollection::GetNextVolume (renderScene, settings);
}
