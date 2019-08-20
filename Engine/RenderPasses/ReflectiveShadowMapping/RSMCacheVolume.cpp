#include "RSMCacheVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

bool RSMCacheVolume::Init (const glm::ivec2& size)
{
	/*
	 * Keep framebuffer size
	*/

	_size = size;

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

	GL::TexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		
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

void RSMCacheVolume::BindForWriting ()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Clear drawing buffer
	*/

	GL::ClearColor (1, 1, 1, 1);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	GL::BindImageTexture (0, _colorBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void RSMCacheVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE10);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

std::vector<PipelineAttribute> RSMCacheVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmCacheMap;

	rsmCacheMap.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmCacheMap.name = "rsmCacheMap";

	rsmCacheMap.value.x = 10;

	attributes.push_back (rsmCacheMap);

	return attributes;
}