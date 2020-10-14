#ifndef HGISAMPLESVOLUME_H
#define HGISAMPLESVOLUME_H

#include "RenderPasses/ScreenSpaceDirectionalOcclusion/SSDOSamplesVolume.h"

class ENGINE_API HGISamplesVolume : public SSDOSamplesVolume
{
public:
	HGISamplesVolume (std::size_t samplesCount);
};

#endif
