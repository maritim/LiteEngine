#include "FrameBuffer2DVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

FrameBuffer2DVolume::FrameBuffer2DVolume () :
	_colorBuffer (0),
	_fbo (0)
{

}

FrameBuffer2DVolume::~FrameBuffer2DVolume ()
{
	Clear ();
}

bool FrameBuffer2DVolume::Init (std::size_t bufferWidth, std::size_t bufferHeight)
{
	/*
	 * Create framebuffer
	*/

	GL::GenFramebuffers (1, &_fbo);
	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Create the 2D color buffer texture
	*/

	GL::GenTextures (1, &_colorBuffer);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);

	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GL::TexImage2D (GL_TEXTURE_2D, 0, GL_RGB8, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, 0);
		
	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);

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

void FrameBuffer2DVolume::BindForWriting ()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Bind final texture as drawing output
	*/

	GL::DrawBuffer (GL_COLOR_ATTACHMENT0);

	/*
	 * Clear drawing buffer
	*/

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);
}

void FrameBuffer2DVolume::BindForBliting ()
{
	/*
	 * Disable writting to framebuffer
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	/*
	 * Bind current framebuffer as read only
	*/

	GL::BindFramebuffer (GL_READ_FRAMEBUFFER, _fbo);

	/*
	 * Bind final texture as drawing output
	*/

	GL::ReadBuffer (GL_COLOR_ATTACHMENT0);
}

void FrameBuffer2DVolume::Clear ()
{
	/*
	 * Bind current framebuffer for cleaning
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Detach textures from color attachments in framebuffer
	*/

	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

	/*
	 * Delete final texture
	*/

	GL::DeleteTextures (1, &_colorBuffer);

	/*
	 * Delete framebuffer
	*/

	GL::DeleteFramebuffers (1, &_fbo);

	/*
	 * Restore default framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);
}
