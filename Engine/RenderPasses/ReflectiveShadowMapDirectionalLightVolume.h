#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "RenderPasses/ReflectiveShadowMapBuffer.h"

#define REFLECTIVE_SHADOW_MAP_MAX_RESOLUTION_WIDTH 512
#define REFLECTIVE_SHADOW_MAP_MAX_RESOLUTION_HEIGHT 512

#define REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT 350

class ReflectiveShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::size_t _cascadedLevels;

	ReflectiveShadowMapBuffer** _shadowMapBuffers;
	std::pair<GLuint, GLuint>* _shadowMapResolutions;

public:
	ReflectiveShadowMapDirectionalLightVolume ();
	~ReflectiveShadowMapDirectionalLightVolume ();

	bool Init (std::size_t cascadedLevels);
	void BindForShadowMapCatch (std::size_t cascadedLevel);
	void EndDrawing ();

	void BindForReading ();

	void Clear ();
};

#endif