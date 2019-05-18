#ifndef FRAMEBUFFERVOLUME_H
#define FRAMEBUFFERVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <glm/vec3.hpp>

class ENGINE_API FrameBuffer2DVolume : public RenderVolumeI
{
protected:
	unsigned int _colorBuffer;
	unsigned int _fbo;
	glm::ivec2 _size;

public:
	FrameBuffer2DVolume ();
	virtual ~FrameBuffer2DVolume ();

	virtual bool Init (const glm::ivec2& size);

	void BindForWriting ();
	void BindForBliting ();
	void BindToBlit ();

	glm::ivec2 GetSize () const;
	unsigned int GetColorTextureID () const;

	void Clear ();
};

#endif