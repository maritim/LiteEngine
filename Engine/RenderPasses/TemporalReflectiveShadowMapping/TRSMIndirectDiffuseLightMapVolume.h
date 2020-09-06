#ifndef TRSMINDIRECTDIFFUSELIGHTMAPVOLUME_H
#define TRSMINDIRECTDIFFUSELIGHTMAPVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

class TRSMIndirectDiffuseLightMapVolume : public FramebufferRenderVolume
{
protected:
	glm::mat4 _viewProjectionMatrix;
	bool _current;

public:
	TRSMIndirectDiffuseLightMapVolume (const Resource<Framebuffer>& framebuffer);

	void SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix);
	void SetCurrent (bool current);

	const glm::mat4& GetViewProjectionMatrix () const;
};

#endif