#ifndef SHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define SHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include "Systems/Camera/Camera.h"

#include "Wrappers/OpenGL/GL.h"

#define SHADOW_MAP_MAX_RESOLUTION_WIDTH 1024		
#define SHADOW_MAP_MAX_RESOLUTION_HEIGHT 1024

#define SHADOW_MAP_FBO_NOT_INIT 330

class ShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::size_t _cascadedLevels;

	GLuint* _shadowMapIndices;
	std::pair<GLuint, GLuint>* _shadowMapResolutions;
	GLuint _frameBufferIndex;

	Camera** _lightCameras;
	float* _shadowMapZEnd;

public:
	ShadowMapDirectionalLightVolume ();
	~ShadowMapDirectionalLightVolume ();

	bool Init (std::size_t cascadedLevels);
	void BindForShadowMapCatch (std::size_t cascadedLevel);
	void EndDrawing ();

	void SetLightCamera (std::size_t cascadedLevel, Camera* lightCamera);
	void SetCameraLimit (std::size_t cascadedLevel, float zLimit);

	Camera* GetLightCamera (std::size_t cascadedLevel);
	float GetCameraLimit (std::size_t cascadedLevel);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif