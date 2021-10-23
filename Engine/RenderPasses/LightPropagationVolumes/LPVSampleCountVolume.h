#ifndef LPVSAMPLECOUNTVOLUME_H
#define LPVSAMPLECOUNTVOLUME_H

#include "LPVGeometryVolume.h"

class LPVSampleCountVolume : public LPVGeometryVolume
{
public:
	virtual bool Init (std::size_t size);

	virtual void BindForWriting ();
};

#endif