#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include <string>

#include "Wrappers/OpenGL/GL.h"

#include "RenderPasses/ReflectiveShadowMapBuffer.h"

#define REFLECTIVE_SHADOW_MAP_MAX_RESOLUTION_WIDTH 1024		
#define REFLECTIVE_SHADOW_MAP_MAX_RESOLUTION_HEIGHT 1024

#define REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT 350

class ReflectiveShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;

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
	void LockShader (int sceneLayers);
};

#endif