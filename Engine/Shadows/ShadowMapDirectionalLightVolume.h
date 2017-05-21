#ifndef SHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define SHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include <string>

#include "Wrappers/OpenGL/GL.h"

#define CASCADED_SHADOW_MAP_LEVELS 4

#define SHADOW_MAP_MAX_RESOLUTION_WIDTH 1024		
#define SHADOW_MAP_MAX_RESOLUTION_HEIGHT 1024

#define SHADOW_MAP_FBO_NOT_INIT 330

class ShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;

	GLuint* _shadowMapIndices;
	std::pair<GLuint, GLuint>* _shadowMapResolutions;
	GLuint _frameBufferIndex;

public:
	ShadowMapDirectionalLightVolume ();
	~ShadowMapDirectionalLightVolume ();

	bool Init ();
	void BindForShadowMapCatch (std::size_t cascadedLevel);
	void EndDrawing ();

	void BindForReading ();
	void LockShader (int sceneLayers);
};

#endif