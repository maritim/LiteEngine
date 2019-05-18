#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Texture.h"
#include <string>
#include <vector>

class ENGINE_API CubeMap : public Texture
{
private:
	std::string _filenames[6];
	unsigned int _gpuIndices[6];
public:
	CubeMap(std::string filename, unsigned int gpuIndex);
	~CubeMap();

	std::vector<std::string> GetFilenames() const;
	std::vector<unsigned int> GetGPUIndices() const;

	void SetFilename(std::string filename, std::size_t index);
	void SetGPUIndexFace(unsigned int gpuIndex, std::size_t index);
};

#endif