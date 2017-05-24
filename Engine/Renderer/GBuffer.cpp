#include "GBuffer.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

GBuffer::GBuffer ()
{

}

GBuffer::~GBuffer ()
{
	/*
	 * Bind current FBO for cleaning
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, m_fbo);

	/*
	 * Union all used textures in a single array
	*/

	GLuint usedTextures[] = { m_textures [0], 
		m_textures [1], 
		m_textures [2], 
		m_textures [3], 
		m_finalTexture, 
		m_depthTexture };

	/*
	 * Detach textures from color attachments in FBO
	*/

	for (std::size_t index = 0; index < ARRAY_SIZE_IN_ELEMENTS(usedTextures) - 1; index++) {
		GL::FramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 0, 0);
	}

	/*
	 * Detach depth texture from attachment in FBO
	*/

	GL::FramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	/*
	 * Delete all textures
	*/

	GL::DeleteTextures (ARRAY_SIZE_IN_ELEMENTS(usedTextures), usedTextures);

	/*
	 * Delete frame buffer
	*/

	GL::DeleteFramebuffers (1, &m_fbo);

	/*
	 * Restore default FBO
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);
}

bool GBuffer::Init(std::size_t bufferWidth, std::size_t bufferHeight)
{
	/*
	 * Create the FBO
	*/

	GL::GenFramebuffers(1, &m_fbo);
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	/*
	 * Create the gbuffer textures
	*/

	GL::GenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
	GL::GenTextures(1, &m_depthTexture);
	GL::GenTextures(1, &m_finalTexture);

	for (std::size_t index = 0 ; index < ARRAY_SIZE_IN_ELEMENTS(m_textures) ; index++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[index]);

		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		GL::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, bufferWidth, bufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		
		GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_textures[index], 0);
	}

	/*
	 * Create depth buffer texture
	*/

	GL::BindTexture(GL_TEXTURE_2D, m_depthTexture);
	GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	/*
	 * Create final texture
	*/

	GL::BindTexture(GL_TEXTURE_2D, m_finalTexture);
	GL::TexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
	GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);

	/*
	 * Check that FBO is ok
	*/

	GLenum status = GL::CheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Console::LogError ("Framebuffer status error: " + status);
		return false;
	}

	/*
	 * Restore default FBO
	*/

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

bool GBuffer::Clear ()
{
	GL::DeleteTextures (ARRAY_SIZE_IN_ELEMENTS (m_textures), m_textures);
	GL::DeleteTextures (1, &m_depthTexture);
	GL::DeleteTextures (1, &m_finalTexture);

	GL::DeleteFramebuffers (1, &m_fbo);

	return 0;
}

void GBuffer::StartFrame()
{
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	GL::DrawBuffer(GL_COLOR_ATTACHMENT4);
	GL::Clear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::BindForGeomPass()
{
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3 };

	GL::DrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
} 

void GBuffer::BindForStencilPass()
{
	/*
	 * Must disable the draw buffers
	*/

	GL::DrawBuffer(GL_NONE);
}

void GBuffer::BindForLightPass()
{
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	
	GL::DrawBuffer(GL_COLOR_ATTACHMENT4);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
		GL::ActiveTexture(GL_TEXTURE0 + i);
		GL::BindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}

void GBuffer::BindForFinalPass()
{
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GL::BindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	GL::ReadBuffer(GL_COLOR_ATTACHMENT4);
} 