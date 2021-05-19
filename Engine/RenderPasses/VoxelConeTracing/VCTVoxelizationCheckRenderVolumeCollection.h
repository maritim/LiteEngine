#ifndef VCTVOXELIZATIONCHECKRENDERVOLUMECOLLECTION_H
#define VCTVOXELIZATIONCHECKRENDERVOLUMECOLLECTION_H

#include "RenderPasses/IterateOverRenderVolumeCollection.h"

class VCTVoxelizationCheckRenderVolumeCollection : public IterateOverRenderVolumeCollection
{
protected:
	bool _check;
	bool _firstTime;

public:
	VCTVoxelizationCheckRenderVolumeCollection (bool check = true);

	RenderVolumeI* GetNextVolume (const RenderScene* renderScene, const RenderSettings&);
};

#endif