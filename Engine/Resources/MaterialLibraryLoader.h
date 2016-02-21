#ifndef MATERIALLIBRARYLOADER_H
#define MATERIALLIBRARYLOADER_H

#include "ResourceLoader.h"

#include <string>

#include "Material/MaterialLibrary.h"

class MaterialLibraryLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
private:
	void ProcessComment(std::ifstream &file);
	Material* SignalNewMaterial(std::ifstream &file, std::string filename, MaterialLibrary* materialLibrary);
	void ProcessShaders(std::ifstream &file, Material* currentMaterial, std::string filename);
	void ProcessCustomAttributes(std::ifstream &file, Material* currentMaterial, std::string filename);
	void ProcessDefaultAttributes(std::ifstream &file, Material* currentMaterial, std::string filename, std::string lineType);
};

#endif