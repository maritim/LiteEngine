#ifndef CASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define CASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/ShadowMap/ShadowMapVolumeI.h"

#include <glm/vec2.hpp>

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Systems/Camera/Camera.h"

class CascadedShadowMapDirectionalLightVolume : public ShadowMapVolumeI
{
protected:
	std::size_t _cascadedLevels;

	std::vector<FramebufferRenderVolume*> _shadowMaps;
	std::vector<glm::ivec2> _shadowMapResolutions;

	std::vector<Camera*> _lightCameras;
	std::vector<float> _shadowMapZEnd;

	float _shadowBias;

	std::vector<PipelineAttribute> _attributes;

public:
	CascadedShadowMapDirectionalLightVolume ();
	~CascadedShadowMapDirectionalLightVolume ();

	virtual bool Init (std::size_t cascadedLevels, const glm::ivec2& resolution);
	void BindForShadowMapCatch (std::size_t cascadedLevel);

	void SetLightCamera (std::size_t cascadedLevel, Camera* lightCamera);
	void SetCameraLimit (std::size_t cascadedLevel, float zLimit);
	void SetShadowBias (float shadowBias);

	Camera* GetLightCamera (std::size_t cascadedLevel);
	float GetCameraLimit (std::size_t cascadedLevel);

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	FramebufferRenderVolume* GetShadowMapVolume (std::size_t cascadedLevel);
	std::size_t GetCascadesCount () const;

	void Clear ();
};

#endif