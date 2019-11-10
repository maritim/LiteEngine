#ifndef CASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define CASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/ShadowMap/ShadowMapVolumeI.h"

#include <glm/vec2.hpp>

#include "RenderPasses/ShadowMap/ShadowMapVolume.h"

#include "Systems/Camera/Camera.h"

class CascadedShadowMapDirectionalLightVolume : public ShadowMapVolumeI
{
protected:
	std::size_t _cascadedLevels;

	std::vector<ShadowMapVolume*> _shadowMaps;
	std::vector<glm::ivec2> _shadowMapResolutions;

	std::vector<Camera*> _lightCameras;
	std::vector<float> _shadowMapZEnd;

public:
	CascadedShadowMapDirectionalLightVolume ();
	~CascadedShadowMapDirectionalLightVolume ();

	virtual bool Init (std::size_t cascadedLevels, const glm::ivec2& resolution);
	void BindForShadowMapCatch (std::size_t cascadedLevel);

	void SetLightCamera (std::size_t cascadedLevel, Camera* lightCamera);
	void SetCameraLimit (std::size_t cascadedLevel, float zLimit);

	Camera* GetLightCamera (std::size_t cascadedLevel);
	float GetCameraLimit (std::size_t cascadedLevel);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	ShadowMapVolume* GetShadowMapVolume (std::size_t cascadedLevel);
	std::size_t GetCascadesCount () const;

	void Clear ();
};

#endif