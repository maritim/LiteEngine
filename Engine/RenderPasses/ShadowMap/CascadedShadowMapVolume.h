#ifndef CASCADESHADOWMAPVOLUME_H
#define CASCADESHADOWMAPVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Systems/Camera/Camera.h"

class CascadedShadowMapVolume : public FramebufferRenderVolume
{
protected:
	std::size_t _cascadeLevels;

	std::vector<Camera*> _lightCameras;
	std::vector<float> _shadowMapZEnd;

public:
	CascadedShadowMapVolume (const Resource<Framebuffer>& framebuffer, std::size_t cascadeLevels);
	~CascadedShadowMapVolume ();

	void SetLightCamera (std::size_t cascadeLevel, Camera* lightCamera);
	void SetCameraLimit (std::size_t cascadeLevel, float zLimit);
	void SetShadowBias (float shadowBias);

	Camera* GetLightCamera (std::size_t cascadedLevel);
	float GetCameraLimit (std::size_t cascadedLevel);

	std::size_t GetCascadeLevels () const;
};

#endif