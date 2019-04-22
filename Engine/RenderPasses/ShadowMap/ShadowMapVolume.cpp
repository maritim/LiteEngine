#include "ShadowMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

bool ShadowMapVolume::Init (std::size_t bufferWidth, std::size_t bufferHeight)
{
	/*
	 * Create framebuffer
	*/

	GL::GenFramebuffers (1, &_fbo);
	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Create the 2D depth buffer texture
	*/

	GL::GenTextures (1, &_colorBuffer);

	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GL::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _colorBuffer, 0);

	/*
	 * Disable draw buffer
	*/

	GL::DrawBuffer (GL_NONE);

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

void ShadowMapVolume::BindForReading ()
{
	/*
	 * Bind color buffer
	*/

	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

void ShadowMapVolume::BindForWriting ()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Clear drawing buffer
	*/

	GL::Clear (GL_DEPTH_BUFFER_BIT);
}

std::vector<PipelineAttribute> ShadowMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Do nothing
	*/

	return attributes;
}
