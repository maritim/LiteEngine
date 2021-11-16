#ifndef LPVPROPAGATIONVOLUME_H
#define LPVPROPAGATIONVOLUME_H

#include "LPVVolume.h"

#define LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT 390

class LPVPropagationVolume : public LPVVolume
{
public:
	virtual bool Init (std::size_t size);

	virtual void BindForWriting (std::size_t index = 0);
};

#endif
