#ifndef SSDOTEMPORALFILTERMAPVOLUME_H
#define SSDOTEMPORALFILTERMAPVOLUME_H

#include "RenderPasses/TemporalFiltering/TemporalFilterMapVolume.h"

class SSDOTemporalFilterMapVolume : public TemporalFilterMapVolume
{
public:
	using TemporalFilterMapVolume::TemporalFilterMapVolume;

	virtual void SetCurrent (bool current);
};

#endif