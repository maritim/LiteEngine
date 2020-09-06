#include "ExponentialShadowMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

// ExponentialShadowMapVolume::ExponentialShadowMapVolume () :
// 	_depthBuffer (0)
// {

// }

// ExponentialShadowMapVolume::~ExponentialShadowMapVolume ()
// {

// }

bool ExponentialShadowMapVolume::Init (std::size_t bufferWidth, std::size_t bufferHeight)
{
	// /*
	//  * Create framebuffer
	// */

	// GL::GenFramebuffers (1, &_fbo);
	// GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	// /*
	//  * Create the 2D color buffer texture
	// */

	// GL::GenTextures (1, &_colorBuffer);
	// GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);

	// GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	// GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	// GL::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// GL::TexImage2D(GL_TEXTURE_2D, 0, GL_R32F, bufferWidth, bufferHeight, 0, GL_RED, GL_FLOAT, NULL);

	// GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);

	// /*
	//  * Create the depth buffer texture
	// */

	// GL::GenTextures (1, &_depthBuffer);
	// GL::BindTexture (GL_TEXTURE_2D, _depthBuffer);

	// GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferHeight, bufferWidth, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	// GL::FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);

	// /*
	//  * Get framebuffer status
	// */

	// GLenum status = GL::CheckFramebufferStatus (GL_FRAMEBUFFER);

	// /*
	//  * Unbind framebuffer
	// */

	// GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	// /*
	//  * Check that framebuffer was successfully initialized
	// */

	// if (status != GL_FRAMEBUFFER_COMPLETE) {
	// 	Console::LogError ("Framebuffer status error: " + status);
	// 	return false;
	// }

	return true;
}

void ExponentialShadowMapVolume::BindForWriting ()
{
	// /*
	//  * Bind framebuffer
	// */

	// GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	// /*
	//  * Enable color attachment
	// */

	// GL::DrawBuffer (GL_COLOR_ATTACHMENT0);

	
	//  * Clear drawing buffer
	

	// float inf = std::numeric_limits<float>::infinity ();

	// GL::ClearColor (inf, inf, inf, 0);
	// GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ExponentialShadowMapVolume::Clear ()
{
	// /*
	//  * Bind current framebuffer for cleaning
	// */

	// GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	// /*
	//  * Detach textures from color attachments in framebuffer
	// */

	// GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

	// /*
	//  * Delete final texture
	// */

	// // GL::DeleteTextures (1, &_colorBuffer);

	
	//  * Detach textures from depth attachments in framebuffer
	

	// GL::FramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	// /*
	//  * Delete depth buffer texture
	// */

	// GL::DeleteTextures (1, &_depthBuffer);

	// /*
	//  * Delete framebuffer
	// */

	// GL::DeleteFramebuffers (1, &_fbo);

	// /*
	//  * Restore default framebuffer
	// */

	// GL::BindFramebuffer (GL_FRAMEBUFFER, 0);
}
