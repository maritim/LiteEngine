#ifndef EXPONENTIALCASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define EXPONENTIALCASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/ShadowMap/CascadedShadowMapVolume.h"

#define EXPONENTIAL_SHADOW_MAP_FBO_NOT_INIT 370

class ExponentialCascadedShadowMapDirectionalLightVolume : public CascadedShadowMapVolume
{
protected:
	float _exponential;

public:
	bool Init (std::size_t cascadedLevels, const glm::ivec2& resolution);

	void SetExponential (float exponential);

	// std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif