#include "BlurMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

bool BlurMapVolume::Init (std::size_t bufferWidth, std::size_t bufferHeight)
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

	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GL::TexImage2D (GL_TEXTURE_2D, 0, GL_RGB16F, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, 0);
		
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

void BlurMapVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE14);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

std::vector<PipelineAttribute> BlurMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute brightMap;

	brightMap.type = PipelineAttribute::AttrType::ATTR_1I;

	brightMap.name = "blurMap";

	brightMap.value.x = 14;

	attributes.push_back (brightMap);

	return attributes;
}
