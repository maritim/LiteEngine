#ifndef SHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define SHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include <string>

class ShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;

	GLuint _shadowMapIndex;
	GLuint _frameBufferIndex;

public:
	ShadowMapDirectionalLightVolume ();
	~ShadowMapDirectionalLightVolume ();

	bool Init ();
	void BindForShadowMapCatch ();
	void EndDrawing ();

	void BindForLightPass ();
	void LockShader (int sceneLayers);
};

#endif