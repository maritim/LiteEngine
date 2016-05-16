#include "CubeMap.h"
#include "Texture.h"

#include <string>
#include <vector>

CubeMap::CubeMap(std::string filename, unsigned int gpuIndex) :
	Texture(filename, nullptr)
{
	_gpuIndex = gpuIndex;
}

CubeMap::~CubeMap ()
{
	
}

std::vector<std::string> CubeMap::GetFilenames() const
{
	std::vector<std::string> result;

	for (std::size_t i = 0;i<6;i++) {
		result.push_back(_filenames[i]);
	}

	return result;
}

std::vector<unsigned int> CubeMap::GetGPUIndices() const
{
	std::vector<unsigned int> result;

	for (std::size_t i =0; i<6;i++) {
		result.push_back(_gpuIndices[i]);
	}

	return result;
}

void CubeMap::SetFilename(std::string filename, std::size_t index)
{
	if (index >= 6) {
		return;
	}

	_filenames[index] = filename;
}

void CubeMap::SetGPUIndexFace(unsigned int gpuIndex, std::size_t index)
{
	if (index >= 6) {
		return;
	}

	_gpuIndices[index] = gpuIndex;
}