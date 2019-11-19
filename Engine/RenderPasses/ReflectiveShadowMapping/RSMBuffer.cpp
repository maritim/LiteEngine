#include "RSMBuffer.h"

#include "Core/Console/Console.h"

RSMBuffer::RSMBuffer () :
	MultipleRenderTargetsVolume (RSMBUFFER_NUM_TEXTURES)
{

}

bool RSMBuffer::Init (const glm::ivec2& size)
{
	/*
	 * Use default initiator
	*/

	if (!MultipleRenderTargetsVolume::Init (size)) {
		return false;
	}

	/*
	 * Change map properties
	*/

	for (std::size_t index = 0; index < m_texturesCount; index++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[index]);

		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	/*
	 * Create the 2D depth buffer texture
	*/

	GL::DeleteTextures (1, &m_depthTexture);

	GL::GenTextures (1, &m_depthTexture);
	GL::BindTexture (GL_TEXTURE_2D, m_depthTexture);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GL::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GL::TexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, _size.x, _size.y);

	GL::BindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	GL::FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	/*
	 * Get framebuffer status
	*/

	GLenum status = GL::CheckFramebufferStatus (GL_FRAMEBUFFER);

	/*
	 * Unbind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	/*
	 * Check that framebuffer was successfully initialized
	*/

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Console::LogError ("Framebuffer status error: " + status);
		return false;
	}

	return true;
}
