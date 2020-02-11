#include "LPVVolume.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

LPVVolume::LPVVolume() :
	_volumeFbo(0),
	_volumeSize(0)
{

}

LPVVolume::~LPVVolume()
{

}

bool LPVVolume::Init(std::size_t volumeSize)
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
		GL::TexImage3D (GL_TEXTURE_3D, 0, GL_RGBA16F, _volumeSize, _volumeSize, _volumeSize,
			0, GL_RGBA, GL_FLOAT, 0);
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
		Console::LogError ("Framebuffer status error: " + status);
		return false;
	}

	/*
	 * Restore default framebuffer
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void LPVVolume::BindForWriting ()
{
	for (std::size_t index = 0; index < 3; index ++) {
		GL::BindImageTexture (index, _volumeTextures [index], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
	}
}

void LPVVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
}

std::vector<PipelineAttribute> LPVVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute volumeTextureR;
	PipelineAttribute volumeTextureG;
	PipelineAttribute volumeTextureB;
	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSize;

	volumeTextureR.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	volumeTextureG.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	volumeTextureB.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSize.type = PipelineAttribute::AttrType::ATTR_3I;

	volumeTextureR.name = "volumeTextureR";
	volumeTextureG.name = "volumeTextureG";
	volumeTextureB.name = "volumeTextureB";
	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSize.name = "volumeSize";

	volumeTextureR.value.x = _volumeTextures [0];
	volumeTextureG.value.x = _volumeTextures [1];
	volumeTextureB.value.x = _volumeTextures [2];
	minVertex.value = _minVertex;
	maxVertex.value = _maxVertex;
	volumeSize.value = glm::vec3 ((float) _volumeSize);

	attributes.push_back (volumeTextureR);
	attributes.push_back (volumeTextureG);
	attributes.push_back (volumeTextureB);
	attributes.push_back (minVertex);
	attributes.push_back (maxVertex);
	attributes.push_back (volumeSize);

	return attributes;
}

void LPVVolume::ClearVolume()
{
	GL::BindFramebuffer(GL_FRAMEBUFFER, _volumeFbo);

	/*
	 * Enable all color attachments correspunding with color textures
	*/

	std::vector<GLenum> DrawBuffers;
	for (std::size_t index = 0; index < 3; index ++) {
		DrawBuffers.push_back ((GLenum)(GL_COLOR_ATTACHMENT0 + index));
	}

	GL::DrawBuffers(DrawBuffers.size (), DrawBuffers.data ());

	GL::ClearColor(0, 0, 0, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LPVVolume::UpdateBoundingBox(const glm::vec3& minVertex, const glm::vec3& maxVertex)
{
	_minVertex = minVertex;
	_maxVertex = maxVertex;

	float sizeX = _maxVertex.x - _minVertex.x;
	float sizeY = _maxVertex.y - _minVertex.y;
	float sizeZ = _maxVertex.z - _minVertex.z;

	float difX, difY, difZ;
	difX = difY = difZ = 0;

	if (sizeX >= sizeY && sizeX >= sizeZ) {
		difY = sizeX - sizeY;
		difZ = sizeX - sizeZ;
	}
	else if (sizeY >= sizeZ) {
		difX = sizeY - sizeX;
		difZ = sizeZ - sizeX;
	}
	else {
		difX = sizeZ - sizeX;
		difY = sizeZ - sizeY;
	}

	_minVertex -= glm::vec3(difX / 2.0f, difY / 2.0f, difZ / 2.0f);
	_maxVertex += glm::vec3(difX / 2.0f, difY / 2.0f, difZ / 2.0f);
}

std::size_t LPVVolume::GetVolumeSize () const
{
	return _volumeSize;
}

void LPVVolume::Clear()
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

	GL::DeleteTextures(3, _volumeTextures);

	/*
	 * Delete framebuffer
	*/

	GL::DeleteFramebuffers(1, &_volumeFbo);
}