#include "VPLCacheVolume.h"

#include <cstring>

#include "Wrappers/OpenGL/GL.h"

VPLCacheVolume::VPLCacheVolume (std::size_t samplesCount) :
	_samplesCount (samplesCount),
	_samplesSSBO (0)
{
	/*
	 * Create samples
	*/

	GL::GenBuffers (1, &_samplesSSBO);
	GL::BindBuffer (GL_SHADER_STORAGE_BUFFER, _samplesSSBO);
	GL::BufferData (GL_SHADER_STORAGE_BUFFER, _samplesCount * 12 * sizeof (float), nullptr, GL_STATIC_DRAW);
	GL::BindBuffer (GL_SHADER_STORAGE_BUFFER, 0);

	/*
	 * Create attributes
	*/

	PipelineAttribute samples;

	samples.type = PipelineAttribute::AttrType::ATTR_SSBO_BLOCK;

	samples.name = "VPLSamples";

	samples.value.x = _samplesSSBO;

	_attributes.push_back (samples);
}

VPLCacheVolume::~VPLCacheVolume ()
{
	GL::DeleteBuffers (1, &_samplesSSBO);
}

const std::vector<PipelineAttribute>& VPLCacheVolume::GetCustomAttributes () const
{
	return _attributes;
}

std::size_t VPLCacheVolume::GetSamplesCount () const
{
	return _samplesCount;
}

VPLCachesVolume::~VPLCachesVolume ()
{
	for (auto& vplCacheVolume : _vplCacheVolumes) {
		delete vplCacheVolume.second;
	}
}

const VPLCacheVolume* VPLCachesVolume::GetVPLCacheVolume (unsigned int vaoIndex) const
{
	auto it = _vplCacheVolumes.find (vaoIndex);

	if (it == _vplCacheVolumes.end ()) {
		return nullptr;
	}

	return it->second;
}

const VPLCacheVolume* VPLCachesVolume::InsertVPLCacheVolume (unsigned int vaoIndex, std::size_t samplesCount)
{
	auto it = _vplCacheVolumes.find (vaoIndex);

	if (it != _vplCacheVolumes.end ()) {
		delete it->second;
	}

	VPLCacheVolume* vplCacheVolume = new VPLCacheVolume (samplesCount);

	_vplCacheVolumes [vaoIndex] = vplCacheVolume;

	return vplCacheVolume;
}
