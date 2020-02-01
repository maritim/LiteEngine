#ifndef TRSMSAMPLESVOLUME_H
#define TRSMSAMPLESVOLUME_H

#include "RenderPasses/ReflectiveShadowMapping/RSMSamplesVolume.h"

class TRSMSamplesVolume : public RSMSamplesVolume
{
public:
	bool Init (std::size_t samplesCount);
};

#endif
