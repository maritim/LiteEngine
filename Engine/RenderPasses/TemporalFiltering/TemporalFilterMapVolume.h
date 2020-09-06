#ifndef TEMPORALFILTERMAPVOLUME_H
#define TEMPORALFILTERMAPVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

class TemporalFilterMapVolume : public FramebufferRenderVolume
{
protected:
	glm::mat4 _viewProjectionMatrix;
	bool _current;

public:
	TemporalFilterMapVolume (const Resource<Framebuffer>& framebuffer);

	void SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix);
	virtual void SetCurrent (bool current);

	const glm::mat4& GetViewProjectionMatrix () const;

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;
};

#endif