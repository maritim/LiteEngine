#include "VCTVoxelizationCheckRenderVolumeCollection.h"

VCTVoxelizationCheckRenderVolumeCollection::VCTVoxelizationCheckRenderVolumeCollection () :
	IterateOverRenderVolumeCollection (1),
	_firstTime (true)
{

}

RenderVolumeI* VCTVoxelizationCheckRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	if (settings.vct_continuous_voxelization == false && _firstTime == false) {
		return nullptr;
	}

	_firstTime = false;

	return IterateOverRenderVolumeCollection::GetNextVolume (renderScene, settings);
}
