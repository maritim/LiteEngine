#ifndef SSDOMAPVOLUME_H
#define SSDOMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class ENGINE_API SSDOMapVolume : public PostProcessMapVolume
{
public:
	void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif