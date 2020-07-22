#ifndef SSDOTEMPORALFILTERMAPVOLUME_H
#define SSDOTEMPORALFILTERMAPVOLUME_H

#include "RenderPasses/TemporalFiltering/TemporalFilterMapVolume.h"

class SSDOTemporalFilterMapVolume : public TemporalFilterMapVolume
{
public:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif