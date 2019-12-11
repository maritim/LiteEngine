#ifndef TAAMAPVOLUME_H
#define TAAMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class TAAMapVolume : public PostProcessMapVolume
{
protected:
	glm::mat4 _viewProjectionMatrix;
	bool _current;

public:
	TAAMapVolume ();

	void SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix);
	void SetCurrent (bool current);

	const glm::mat4& GetViewProjectionMatrix () const;

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif