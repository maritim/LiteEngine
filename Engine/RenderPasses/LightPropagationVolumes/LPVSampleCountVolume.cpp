#include "LPVSampleCountVolume.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

bool LPVSampleCountVolume::Init(std::size_t volumeSize)
{
	/*
	 * Keep new current volume size
	*/

	_volumeSize = volumeSize;

	/*
	 * Create an fbo for clearing the 3D texture.
	*/

	GL::GenFramebuffers(1, &_volumeFbo);

	GL::BindFramebuffer (GL_FRAMEBUFFER, _volumeFbo);

	/*
	 * Create the 3D texture to keep the voxel volume
	*/

	GL::GenTextures (1, _volumeTextures);

	GL::BindTexture (GL_TEXTURE_3D, _volumeTextures [0]);
	GL::TexImage3D (GL_TEXTURE_3D, 0, GL_R32UI, _volumeSize, _volumeSize, _volumeSize,
		0, GL_RED_INTEGER,  GL_UNSIGNED_INT, 0);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	GL::FramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _volumeTextures [0], 0);

	/*
	 * Check that framebuffer is ok
	*/

	GLenum status = GL::CheckFramebufferStatus (GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Console::LogError ("Framebuffer status error: " + std::to_string(status));
		return false;
	}

	/*
	 * Restore default framebuffer
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	_attributes.clear ();

	PipelineAttribute sampleCountTexture;

	sampleCountTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;

	sampleCountTexture.name = "sampleCountTexture";

	sampleCountTexture.value.x = _volumeTextures [0];

	_attributes.push_back (sampleCountTexture);

	return true;
}

void LPVSampleCountVolume::BindForWriting ()
{
	GL::BindImageTexture (4, _volumeTextures [0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
}
