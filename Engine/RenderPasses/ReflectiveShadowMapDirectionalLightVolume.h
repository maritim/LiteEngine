#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/ShadowMap/ShadowMapVolumeI.h"

#include "Wrappers/OpenGL/GL.h"

#include "RenderPasses/ReflectiveShadowMapBuffer.h"

#define REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT 350

class ReflectiveShadowMapDirectionalLightVolume : public ShadowMapVolumeI
{
protected:
	std::size_t _width;
	std::size_t _height;
	std::size_t _cascadedLevels;

	ReflectiveShadowMapBuffer** _shadowMapBuffers;
	std::pair<GLuint, GLuint>* _shadowMapResolutions;

public:
	ReflectiveShadowMapDirectionalLightVolume ();
	~ReflectiveShadowMapDirectionalLightVolume ();

	virtual bool Init (std::size_t width, std::size_t height, std::size_t cascadedLevels);

	bool Init (std::size_t cascadedLevels);
	void BindForShadowMapCatch (std::size_t cascadedLevel);
	void EndDrawing ();

	void BindForReading ();

	void Clear ();
};

#endif