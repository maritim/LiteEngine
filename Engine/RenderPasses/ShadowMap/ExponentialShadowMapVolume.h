#ifndef EXPONENTIALSHADOWMAPVOLUME_H
#define EXPONENTIALSHADOWMAPVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Wrappers/OpenGL/GL.h"

class ExponentialShadowMapVolume : public FramebufferRenderVolume
{
protected:
	unsigned int _depthBuffer;

public:
	// ExponentialShadowMapVolume ();
	// ~ExponentialShadowMapVolume ();

	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForWriting ();

	void Clear ();
};

#endif