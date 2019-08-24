#include "VCTDebugCheckRenderVolumeCollection.h"

VCTDebugCheckRenderVolumeCollection::VCTDebugCheckRenderVolumeCollection () :
	IterateOverRenderVolumeCollection (1)
{

}

RenderVolumeI* VCTDebugCheckRenderVolumeCollection::GetNextVolume (const RenderScene* renderScene, const RenderSettings& settings)
{
	if (settings.vct_debug_show_voxels == true) {
		return nullptr;
	}

	return IterateOverRenderVolumeCollection::GetNextVolume (renderScene, settings);
}
