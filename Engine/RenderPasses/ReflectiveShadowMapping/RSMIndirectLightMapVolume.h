#ifndef RSMINDIRECTLIGHTMAPVOLUME_H
#define RSMINDIRECTLIGHTMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class RSMIndirectLightMapVolume : public PostProcessMapVolume
{
protected:
	glm::mat4 _viewProjectionMatrix;
	bool _current;

public:
	RSMIndirectLightMapVolume ();

	void SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix);
	void SetCurrent (bool current);

	const glm::mat4& GetViewProjectionMatrix () const;

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif