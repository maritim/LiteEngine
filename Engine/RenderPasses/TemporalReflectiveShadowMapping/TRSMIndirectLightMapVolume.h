#ifndef TRSMINDIRECTLIGHTMAPVOLUME_H
#define TRSMINDIRECTLIGHTMAPVOLUME_H

#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectLightMapVolume.h"

class TRSMIndirectLightMapVolume : public RSMIndirectLightMapVolume
{
protected:
	glm::mat4 _viewProjectionMatrix;
	bool _current;

public:
	TRSMIndirectLightMapVolume ();

	void SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix);
	void SetCurrent (bool current);

	const glm::mat4& GetViewProjectionMatrix () const;

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif