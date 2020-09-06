#ifndef LPVGEOMETRYVOLUME_H
#define LPVGEOMETRYVOLUME_H

#include "LPVVolume.h"

#define LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT 390

class LPVGeometryVolume : public LPVVolume
{
public:
	virtual bool Init (std::size_t size);

	virtual void BindForWriting ();

	virtual void ClearVolume();

	virtual void Clear ();
};

#endif