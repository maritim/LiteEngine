#include "MultipleRenderTargetsVolume.h"

#include <vector>

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

MultipleRenderTargetsVolume::MultipleRenderTargetsVolume(std::size_t numTextures) :
	m_textures (new GLuint [numTextures]),
	m_texturesCount (numTextures),
	m_finalTextureIndex (numTextures)
{

}

MultipleRenderTargetsVolume::~MultipleRenderTargetsVolume ()
{
	/*
	 * Bind current FBO for cleaning
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, m_fbo);

	/*
	 * Union all used textures in a single array
	*/

	std::vector<GLuint> usedTextures;

	for (std::size_t index = 0; index < m_texturesCount; index ++) {
		usedTextures.push_back (m_textures [index]);
	}

	usedTextures.push_back (m_finalTexture);
	usedTextures.push_back (m_depthTexture);

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

	GL::DeleteTextures (usedTextures.size(), usedTextures.data());

	/*
	 * Delete frame buffer
	*/

	GL::DeleteFramebuffers (1, &m_fbo);

	/*
	 * Restore default FBO
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);
}

bool MultipleRenderTargetsVolume::Init(std::size_t bufferWidth, std::size_t bufferHeight)
{
	/*
	 * Create the FBO
	*/

	GL::GenFramebuffers(1, &m_fbo);
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	/*
	 * Create the all drawable textures for multiple render targets
	*/

	GL::GenTextures(m_texturesCount, m_textures);
	GL::GenTextures(1, &m_depthTexture);
	GL::GenTextures(1, &m_finalTexture);

	for (std::size_t index = 0 ; index < m_texturesCount ; index++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[index]);

		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

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
	GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_finalTextureIndex, GL_TEXTURE_2D, m_finalTexture, 0);

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

bool MultipleRenderTargetsVolume::Clear ()
{
	/*
	 * Delete all textures
	*/

	GL::DeleteTextures (m_texturesCount, m_textures);
	GL::DeleteTextures (1, &m_depthTexture);
	GL::DeleteTextures (1, &m_finalTexture);

	/*
	 * Delete framebuffer
	*/

	GL::DeleteFramebuffers (1, &m_fbo);

	return true;
}

void MultipleRenderTargetsVolume::StartFrame()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	/*
	 * Bind final texture as drawing output
	*/

	GL::DrawBuffer(GL_COLOR_ATTACHMENT0 + m_finalTextureIndex);

	/*
	 * Clear drawing buffer
	*/

	GL::Clear(GL_COLOR_BUFFER_BIT);
}

void MultipleRenderTargetsVolume::BindForFinalPass()
{
	/*
	 * Disable writting to framebuffer
	*/

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	/*
	 * Bind current framebuffer with read only
	*/

	GL::BindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	/*
	 * Bind final texture as drawing output
	*/

	GL::ReadBuffer(GL_COLOR_ATTACHMENT0 + m_finalTextureIndex);
}

void MultipleRenderTargetsVolume::BindForReading ()
{
	/*
	 * Bind current framebuffer for writting
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, m_fbo);

	/*
	 * Bind final texture as drawing output
	*/

	GL::DrawBuffer (GL_COLOR_ATTACHMENT0 + m_finalTextureIndex);

	/*
	 * Bind all color textures for reading
	*/

	for (std::size_t i = 0; i < m_texturesCount; i++) {
		GL::ActiveTexture (GL_TEXTURE0 + i);
		GL::BindTexture (GL_TEXTURE_2D, m_textures [i]);
	}
}

void MultipleRenderTargetsVolume::BindForWriting ()
{	
	/*
	 * Bind current framebuffer for writting
	*/

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	/*
	 * Enable all color attachments correspunding with color textures
	*/

	std::vector<GLenum> DrawBuffers;
	for (std::size_t index = 0; index < m_texturesCount; index ++) {
		DrawBuffers.push_back ((GLenum)(GL_COLOR_ATTACHMENT0 + index));
	}

	GL::DrawBuffers(DrawBuffers.size (), DrawBuffers.data ());
}

std::vector<PipelineAttribute> MultipleRenderTargetsVolume::GetCustomAttributes () const
{
	return std::vector<PipelineAttribute> ();
}
