#include "VoxelVolume.h"

#include "Renderer/Pipeline.h"

#include "Settings/GeneralSettings.h"

VoxelVolume::VoxelVolume() :
	_volumeTexture(0),
	_volumeFbo(0),
	_volumeSize(0)
{

}

VoxelVolume::~VoxelVolume()
{
	Clear();
}

void VoxelVolume::Init(std::size_t volumeSize)
{
	/*
	* Clear current volume if needed
	*/

	Clear();

	/*
	* Keep new current volume size
	*/

	_volumeSize = volumeSize;

	/*
	* Create the 3D texture to keep the voxel volume
	*/

	GL::GenTextures (1, &_volumeTexture);
	GL::BindTexture (GL_TEXTURE_3D, _volumeTexture);
	GL::TexImage3D (GL_TEXTURE_3D, 0, GL_RGBA8, _volumeSize, _volumeSize, _volumeSize,
		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GL::TexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	/*
	* Initialize mipmaps
	*/

	GL::GenerateMipmap (GL_TEXTURE_3D);

	/*
	* Create an fbo for clearing the 3D texture.
	*/

	GL::GenFramebuffers(1, &_volumeFbo);

	GL::BindFramebuffer (GL_FRAMEBUFFER, _volumeFbo);
	GL::FramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _volumeTexture, 0);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VoxelVolume::BindForWriting ()
{
	GL::BindImageTexture (0, _volumeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void VoxelVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE10);
	GL::BindTexture (GL_TEXTURE_3D, _volumeTexture);
}

std::vector<PipelineAttribute> VoxelVolume::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute volumeTexture;
	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSize;
	PipelineAttribute volumeMipmapLevel;

	volumeTexture.type = PipelineAttribute::AttrType::ATTR_1I;
	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSize.type = PipelineAttribute::AttrType::ATTR_3I;
	volumeMipmapLevel.type = PipelineAttribute::AttrType::ATTR_1I;

	volumeTexture.name = "volumeTexture";
	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSize.name = "volumeSize";
	volumeMipmapLevel.name = "volumeMipmapLevel";

	volumeTexture.value.x = 10;
	minVertex.value = _minVertex;
	maxVertex.value = _maxVertex;
	volumeSize.value = glm::vec3 ((float) _volumeSize);
	volumeMipmapLevel.value.x = GeneralSettings::Instance ()->GetIntValue ("VoxelVolumeMipmapLevel");

	attributes.push_back (volumeTexture);
	attributes.push_back (minVertex);
	attributes.push_back (maxVertex);
	attributes.push_back (volumeSize);
	attributes.push_back (volumeMipmapLevel);

	return attributes;
}

void VoxelVolume::ClearVoxels()
{
	GL::BindFramebuffer(GL_FRAMEBUFFER, _volumeFbo);
	GL::ClearColor(0, 0, 0, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VoxelVolume::UpdateBoundingBox(const glm::vec3& minVertex, const glm::vec3& maxVertex)
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

void VoxelVolume::BindForWriting (std::size_t mipmap)
{
	GL::BindImageTexture (0, _volumeTexture, mipmap, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void VoxelVolume::Clear()
{
	GL::DeleteTextures(1, &_volumeTexture);
	GL::DeleteFramebuffers(1, &_volumeFbo);
}