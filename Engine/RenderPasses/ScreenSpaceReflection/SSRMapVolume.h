#ifndef SSRMAPVOLUME_H
#define SSRMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class SSRMapVolume : public PostProcessMapVolume
{
public:
	void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif