#ifndef SHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define SHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include <string>

class ShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::string _shaderName;

	GLuint _shadowMapIndex;
	GLuint _frameBufferIndex;

public:
	ShadowMapDirectionalLightVolume ();
	~ShadowMapDirectionalLightVolume ();

	bool Init ();
	void BindForShadowMapCatch ();
	void EndDrawing ();

	void BindForLightPass ();
};

#endif