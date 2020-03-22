#ifndef TRSMINDIRECTDIFFUSELIGHTMAPVOLUME_H
#define TRSMINDIRECTDIFFUSELIGHTMAPVOLUME_H

#include "RenderPasses/IndirectDiffuseLightMapVolume.h"

class TRSMIndirectDiffuseLightMapVolume : public IndirectDiffuseLightMapVolume
{
protected:
	glm::mat4 _viewProjectionMatrix;
	bool _current;

public:
	TRSMIndirectDiffuseLightMapVolume ();

	void SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix);
	void SetCurrent (bool current);

	const glm::mat4& GetViewProjectionMatrix () const;

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif