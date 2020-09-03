#include "MultipleRenderTargetsVolume.h"

#include <vector>

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

MultipleRenderTargetsVolume::MultipleRenderTargetsVolume(std::size_t numTextures) :
	m_fbo (0),
	m_textures (new GLuint [numTextures]),
	m_depthTexture (0),
	m_texturesCount (numTextures),
	_size (0)
{

}

MultipleRenderTargetsVolume::~MultipleRenderTargetsVolume ()
{
	delete[] m_textures;
}

bool MultipleRenderTargetsVolume::Init(const glm::ivec2& size)
{
	/*
	 * Keep framebuffer size
	*/

	_size = size;

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

	for (std::size_t index = 0 ; index < m_texturesCount ; index++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[index]);

		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GL::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _size.x, _size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		
		GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_textures[index], 0);
	}

	/*
	 * Create depth buffer texture
	*/

	GL::BindTexture(GL_TEXTURE_2D, m_depthTexture);
	GL::TexStorage2D (GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _size.x, _size.y);
	GL::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

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

void MultipleRenderTargetsVolume::Clear ()
{
	/*
	 * Check if it's not initialized
	*/

	if (m_fbo == 0) {
		return;
	}

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

	/*
	 * Reset FBO
	*/

	m_fbo = 0;
}

void MultipleRenderTargetsVolume::GenerateMipmap ()
{
	/*
	 * Generate mipmap for each texture
	*/

	for (std::size_t index = 0 ; index < m_texturesCount ; index++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[index]);

		GL::GenerateMipmap (GL_TEXTURE_2D);
	}
}

void MultipleRenderTargetsVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
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

	/*
	 * Clear framebuffer
	*/

	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void MultipleRenderTargetsVolume::BindDraw ()
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

unsigned int MultipleRenderTargetsVolume::GetColorBuffer (std::size_t index) const
{
	return m_textures [index];
}

unsigned int MultipleRenderTargetsVolume::GetDepthBuffer () const
{
	return m_depthTexture;
}

glm::ivec2 MultipleRenderTargetsVolume::GetSize () const
{
	return _size;
}
