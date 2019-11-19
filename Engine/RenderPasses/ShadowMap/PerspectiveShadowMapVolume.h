#ifndef PERSPECTIVESHADOWMAPVOLUME_H
#define PERSPECTIVESHADOWMAPVOLUME_H

#include "RenderPasses/ShadowMap/ShadowMapVolume.h"

#include "Cameras/PerspectiveCamera.h"

class PerspectiveShadowMapVolume : public ShadowMapVolume
{
protected:
	PerspectiveCamera* _lightCamera;
	float _shadowBias;

public:
	PerspectiveShadowMapVolume ();

	bool Init (PerspectiveCamera* lightCamera, const glm::ivec2& size);

	void Clear ();

	void SetShadowBias (float shadowBias);

	PerspectiveCamera* GetLightCamera ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif