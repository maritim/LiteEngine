#include "LPVGeometryVolume.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

bool LPVGeometryVolume::Init(std::size_t volumeSize)
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
	GL::TexImage3D (GL_TEXTURE_3D, 0, GL_RGBA8, _volumeSize, _volumeSize, _volumeSize,
		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
		Console::LogError ("Framebuffer status error: " + status);
		return false;
	}

	/*
	 * Restore default framebuffer
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	_attributes.clear ();

	PipelineAttribute geometryTexture;
	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSizeAttribute;

	geometryTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSizeAttribute.type = PipelineAttribute::AttrType::ATTR_3I;

	geometryTexture.name = "geometryTexture";
	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSizeAttribute.name = "volumeSize";

	geometryTexture.value.x = _volumeTextures [0];
	minVertex.value = _minVertex;
	maxVertex.value = _maxVertex;
	volumeSizeAttribute.value = glm::vec3 ((float) _volumeSize);

	_attributes.push_back (geometryTexture);
	_attributes.push_back (minVertex);
	_attributes.push_back (maxVertex);
	_attributes.push_back (volumeSizeAttribute);

	return true;
}

void LPVGeometryVolume::BindForWriting ()
{
	GL::BindImageTexture (3, _volumeTextures [0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void LPVGeometryVolume::ClearVolume()
{
	GL::BindFramebuffer(GL_FRAMEBUFFER, _volumeFbo);

	GL::ClearColor(0, 0, 0, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LPVGeometryVolume::Clear()
{
	/*
	 * Bind current framebuffer for cleaning
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _volumeFbo);

	/*
	 * Detach texture from attachment in framebuffer
	*/

	GL::FramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);

	/*
	 * Delete texture
	*/

	GL::DeleteTextures(1, _volumeTextures);

	/*
	 * Delete framebuffer
	*/

	GL::DeleteFramebuffers(1, &_volumeFbo);
}
