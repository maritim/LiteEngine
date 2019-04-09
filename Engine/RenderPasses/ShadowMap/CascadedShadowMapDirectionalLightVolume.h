#ifndef CASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define CASCADEDSHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/ShadowMap/ShadowMapVolumeI.h"

#include "RenderPasses/ShadowMap/ShadowMapVolume.h"

#include "Systems/Camera/Camera.h"

#include "Wrappers/OpenGL/GL.h"

#define SHADOW_MAP_MAX_RESOLUTION_WIDTH 1024		
#define SHADOW_MAP_MAX_RESOLUTION_HEIGHT 1024

#define SHADOW_MAP_FBO_NOT_INIT 330

class CascadedShadowMapDirectionalLightVolume : public ShadowMapVolumeI
{
protected:
	std::size_t _cascadedLevels;

	std::vector<ShadowMapVolume*> _shadowMaps;
	std::vector<std::pair<GLuint, GLuint>> _shadowMapResolutions;

	std::vector<Camera*> _lightCameras;
	std::vector<float> _shadowMapZEnd;

public:
	CascadedShadowMapDirectionalLightVolume ();
	~CascadedShadowMapDirectionalLightVolume ();

	bool Init (std::size_t cascadedLevels);
	void BindForShadowMapCatch (std::size_t cascadedLevel);
	void EndDrawing ();

	void SetLightCamera (std::size_t cascadedLevel, Camera* lightCamera);
	void SetCameraLimit (std::size_t cascadedLevel, float zLimit);

	Camera* GetLightCamera (std::size_t cascadedLevel);
	float GetCameraLimit (std::size_t cascadedLevel);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void Clear ();
};

#endif