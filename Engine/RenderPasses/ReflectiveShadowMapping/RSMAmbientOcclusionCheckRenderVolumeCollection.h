#ifndef RSMAMBIENTOCCLUSIONCHECKRENDERVOLUMECOLLECTION_H
#define RSMAMBIENTOCCLUSIONCHECKRENDERVOLUMECOLLECTION_H

#include "RenderPasses/IterateOverRenderVolumeCollection.h"

class RSMAmbientOcclusionCheckRenderVolumeCollection : public IterateOverRenderVolumeCollection
{
public:
	RSMAmbientOcclusionCheckRenderVolumeCollection ();

	RenderVolumeI* GetNextVolume (const RenderScene* renderScene, const RenderSettings&);
};

#endif