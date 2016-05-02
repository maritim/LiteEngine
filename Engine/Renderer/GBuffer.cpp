#include "GBuffer.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

GBuffer::GBuffer ()
{

}

GBuffer::~GBuffer ()
{

}

bool GBuffer::Init(unsigned int bufferWidth, unsigned int bufferHeight)
{
	// Create the FBO
	GL::GenFramebuffers(1, &m_fbo);
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Create the gbuffer textures
	GL::GenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
	GL::GenTextures(1, &m_depthTexture);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures) ; i++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[i]);
		GL::TexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
		GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	// depth
	GL::BindTexture(GL_TEXTURE_2D, m_depthTexture);
	GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	GL::DrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Console::LogError ("Framebuffer status error" + status);
		return false;
	}

	// restore default FBO
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
} 

void GBuffer::BindForWriting()
{
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	GL::DrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
}

void GBuffer::BindForReading()
{
	GL::BindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
} 
