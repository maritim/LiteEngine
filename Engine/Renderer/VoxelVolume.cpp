#include "VoxelVolume.h"

#include "Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Managers/ShaderManager.h"
#include "Systems/Window/Window.h"

VoxelVolume::VoxelVolume() :
	_volumeTexture (0),
	_volumeFbo (0),
	_volumeSize (0)
{
	/*
	 * Voxelization shader init
	*/

	ShaderManager::Instance()->AddShader("VOXELIZATION_SHADER",
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");
}

VoxelVolume::~VoxelVolume()
{
	Clear ();
}

void VoxelVolume::Init(std::size_t volumeSize)
{
	/*
	 * Clear current volume if needed
	*/

	Clear ();

	/*
	 * Keep new current volume size
	*/

	_volumeSize = volumeSize;

	/*
	 * Create the 3D texture to keep the voxel volume
	*/

	GL::GenTextures(1, &_volumeTexture);
	GL::BindTexture(GL_TEXTURE_3D, _volumeTexture);
	GL::TexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, _volumeSize, _volumeSize, _volumeSize,
		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	/*
	 * Create an fbo for clearing the 3D texture.
	*/

	GL::GenFramebuffers(1, &_volumeFbo);
	GL::BindFramebuffer(GL_FRAMEBUFFER, _volumeFbo);
	GL::FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _volumeTexture, 0);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VoxelVolume::StartVoxelizationPass()
{
	/*
	 * Render to window but mask out all color.
	*/

	GL::ColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask(GL_FALSE);
	GL::Viewport(0, 0, _volumeSize, _volumeSize);

	/*
	 * Lock voxelization shader for geomtry rendering
	*/

	Pipeline::LockShader(ShaderManager::Instance()->GetShader("VOXELIZATION_SHADER"));
}

void VoxelVolume::EndVoxelizationPass ()
{
	// GL::MemoryBarrier(GL_ALL_BARRIER_BITS);

	/*
	 * Clear settings
	*/

	GL::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());
	GL::ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask(GL_TRUE);

	/*
	 * Unlock voxelization shader
	*/

	Pipeline::UnlockShader();
}

void VoxelVolume::BindForVoxelizationPass ()
{
	GL::BindImageTexture(0, _volumeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	GL::Uniform1i(0, 0);

	UpdateVoxelizationPipelineAttributes ();
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

	if (sizeX > sizeY && sizeX > sizeZ) {
		difY = sizeX - sizeY;
		difZ = sizeX - sizeZ;
	}
	else if (sizeY > sizeZ) {
		difX = sizeY - sizeX;
		difZ = sizeZ - sizeX;
	}
	else {
		difX = sizeZ - sizeX;
		difY = sizeZ - sizeY;
	}

	_minVertex -= glm::vec3 (difX / 2.0f, difY / 2.0f, difZ / 2.0f);
	_maxVertex += glm::vec3 (difX / 2.0f, difY / 2.0f, difZ / 2.0f);
}

void VoxelVolume::StartRayTracePass()
{
	/*
	 * Load voxel ray trace shader
	*/

	ShaderManager::Instance()->AddShader("VOXEL_RAY_TRACE_SHADER",
		"Assets/Shaders/Voxelize/voxelRayTraceVertex.glsl",
		"Assets/Shaders/Voxelize/voxelRayTraceFragment.glsl");

	/* 
	 * Use voxel ray trace shader
	*/

	Pipeline::SetShader(ShaderManager::Instance()->GetShader("VOXEL_RAY_TRACE_SHADER"));
}

void VoxelVolume::BindForRayTracePass()
{
	Pipeline::SendCamera (Camera::Main ());
	Pipeline::ClearObjectTransform ();
	Pipeline::UpdateMatrices (ShaderManager::Instance()->GetShader("VOXEL_RAY_TRACE_SHADER"));

	UpdateVoxelRayTracePipelineAttributes();
}

void VoxelVolume::UpdateVoxelizationPipelineAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute minPosition;
	PipelineAttribute maxPosition;
	PipelineAttribute volumeSize;

	minPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	maxPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSize.type = PipelineAttribute::AttrType::ATTR_3I;

	minPosition.name = "minPosition";
	maxPosition.name = "maxPosition";
	volumeSize.name = "volumeSize";

	minPosition.value = _minVertex;
	maxPosition.value = _maxVertex;
	volumeSize.value = glm::vec3 ((float) _volumeSize);

	attributes.push_back(minPosition);
	attributes.push_back(maxPosition);
	attributes.push_back(volumeSize);

	Pipeline::SendCustomAttributes("VOXELIZATION_SHADER", attributes);
}

void VoxelVolume::UpdateVoxelRayTracePipelineAttributes()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute volumeTexture;
	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSize;

	volumeTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSize.type = PipelineAttribute::AttrType::ATTR_3I;

	volumeTexture.name = "volumeTexture";
	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSize.name = "volumeSize";

	volumeTexture.value.x = (float) _volumeTexture;
	minVertex.value = _minVertex;
	maxVertex.value = _maxVertex;
	volumeSize.value = glm::vec3 ((float) _volumeSize);

	attributes.push_back(volumeTexture);
	attributes.push_back(minVertex);
	attributes.push_back(maxVertex);
	attributes.push_back(volumeSize);

	Pipeline::SendCustomAttributes("VOXEL_RAY_TRACE_SHADER", attributes);
}

void VoxelVolume::Clear()
{
	GL::DeleteTextures(1, &_volumeTexture);
	GL::DeleteFramebuffers(1, &_volumeFbo);
}