#ifndef MULTIPLERENDERTARGETSBUFFER_H
#define MULTIPLERENDERTARGETSBUFFER_H

#include "Renderer/RenderVolumeI.h"

#include "Wrappers/OpenGL/GL.h"

class MultipleRenderTargetsVolume : public RenderVolumeI
{
protected:
	GLuint m_fbo;
	GLuint* m_textures;
	GLuint m_depthTexture;
	std::size_t m_texturesCount;

public:
	MultipleRenderTargetsVolume (std::size_t numTextures); 
	virtual ~MultipleRenderTargetsVolume ();

	virtual bool Init(std::size_t screenWidth, std::size_t screenHeight);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

    unsigned int GetDepthBuffer () const;
protected:
	void Clear ();
};

#endif