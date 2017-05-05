#ifndef SHADOWMAPDIRECTIONALLIGHTVOLUME_H
#define SHADOWMAPDIRECTIONALLIGHTVOLUME_H

#include "ShadowMapVolume.h"

#include <string>

class ShadowMapDirectionalLightVolume : public ShadowMapVolume
{
protected:
	std::string _shaderName;

	unsigned int _shadowMapIndex;
	unsigned int _frameBufferIndex;

public:
	ShadowMapDirectionalLightVolume ();
	~ShadowMapDirectionalLightVolume ();

	bool Init ();
	void BindForShadowMapCatch ();
	void EndDrawing ();
};

#endif