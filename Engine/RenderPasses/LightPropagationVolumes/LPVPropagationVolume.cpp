#include "LPVPropagationVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

bool LPVPropagationVolume::Init(std::size_t volumeSize)
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

	GL::GenTextures (3, _volumeTextures);

	for (std::size_t index = 0; index < 3; index ++) {
		GL::BindTexture (GL_TEXTURE_3D, _volumeTextures [index]);
		GL::TexImage3D (GL_TEXTURE_3D, 0, GL_RGBA32F, _volumeSize, _volumeSize, _volumeSize,
			0, GL_RGBA,  GL_FLOAT, 0);
		GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		GL::FramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, _volumeTextures [index], 0);
	}

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

	PipelineAttribute volumeTextureR;
	PipelineAttribute volumeTextureG;
	PipelineAttribute volumeTextureB;
	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSizeAttribute;

	volumeTextureR.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	volumeTextureG.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	volumeTextureB.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSizeAttribute.type = PipelineAttribute::AttrType::ATTR_3I;

	volumeTextureR.name = "volumeTextureR";
	volumeTextureG.name = "volumeTextureG";
	volumeTextureB.name = "volumeTextureB";
	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSizeAttribute.name = "volumeSize";

	volumeTextureR.value.x = _volumeTextures [0];
	volumeTextureG.value.x = _volumeTextures [1];
	volumeTextureB.value.x = _volumeTextures [2];
	volumeSizeAttribute.value = glm::vec3 ((float) _volumeSize);

	_attributes.push_back (volumeTextureR);
	_attributes.push_back (volumeTextureG);
	_attributes.push_back (volumeTextureB);
	_attributes.push_back (minVertex);
	_attributes.push_back (maxVertex);
	_attributes.push_back (volumeSizeAttribute);

	return true;
}

void LPVPropagationVolume::BindForWriting (std::size_t index)
{
	for (std::size_t imageIndex = 0; imageIndex < 3; imageIndex ++) {
		GL::BindImageTexture (index + imageIndex, _volumeTextures [imageIndex], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}
}

