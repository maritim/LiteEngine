#ifndef VCTDEBUGCHECKRENDERVOLUMECOLLECTION_H
#define VCTDEBUGCHECKRENDERVOLUMECOLLECTION_H

#include "RenderPasses/IterateOverRenderVolumeCollection.h"

class VCTDebugCheckRenderVolumeCollection : public IterateOverRenderVolumeCollection
{
public:
	VCTDebugCheckRenderVolumeCollection ();

	RenderVolumeI* GetNextVolume (const RenderScene* renderScene, const RenderSettings&);
};

#endif