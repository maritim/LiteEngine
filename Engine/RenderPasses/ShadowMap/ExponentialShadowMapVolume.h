#ifndef EXPONENTIALSHADOWMAPVOLUME_H
#define EXPONENTIALSHADOWMAPVOLUME_H

#include "ShadowMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

class ExponentialShadowMapVolume : public ShadowMapVolume
{
protected:
	unsigned int _depthBuffer;

public:
	ExponentialShadowMapVolume ();
	~ExponentialShadowMapVolume ();

	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForWriting ();

	void Clear ();
};

#endif