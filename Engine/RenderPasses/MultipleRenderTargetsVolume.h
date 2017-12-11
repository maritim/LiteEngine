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
	GLuint m_finalTexture;
	std::size_t m_texturesCount;
	std::size_t m_finalTextureIndex;

public:
	MultipleRenderTargetsVolume (std::size_t numTextures); 
	virtual ~MultipleRenderTargetsVolume ();

	virtual bool Init(std::size_t screenWidth, std::size_t screenHeight);
	bool Clear ();

	void StartFrame();
	void BindForFinalPass();

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif