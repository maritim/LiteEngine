#ifndef MULTIPLERENDERTARGETSBUFFER_H
#define MULTIPLERENDERTARGETSBUFFER_H

#include "Renderer/RenderVolumeI.h"

#include <glm/vec3.hpp>

#include "Wrappers/OpenGL/GL.h"

class MultipleRenderTargetsVolume : public RenderVolumeI
{
protected:
	GLuint m_fbo;
	GLuint* m_textures;
	GLuint m_depthTexture;
	std::size_t m_texturesCount;
	glm::ivec2 _size;

public:
	MultipleRenderTargetsVolume (std::size_t numTextures); 
	virtual ~MultipleRenderTargetsVolume ();

	virtual bool Init(const glm::ivec2& size);

	void GenerateMipmap ();

	void BindForReading ();
	void BindForWriting ();
	void BindDraw ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	unsigned int GetColorBuffer (std::size_t index) const;
    unsigned int GetDepthBuffer () const;

    glm::ivec2 GetSize () const;

	void Clear ();
};

#endif