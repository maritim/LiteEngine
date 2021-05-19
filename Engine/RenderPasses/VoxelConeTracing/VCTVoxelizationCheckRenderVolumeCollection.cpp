#include "VCTVoxelizationCheckRenderVolumeCollection.h"

VCTVoxelizationCheckRenderVolumeCollection::VCTVoxelizationCheckRenderVolumeCollection (bool check) :
	IterateOverRenderVolumeCollection (1),
	_check (check),
	_firstTime (true)
{

}

RenderVolumeI* VCTVoxelizationCheckRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	if (settings.vct_continuous_voxelization == !_check && _firstTime == false) {
		return nullptr;
	}

	_firstTime = false;

	return IterateOverRenderVolumeCollection::GetNextVolume (renderScene, settings);
}
