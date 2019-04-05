#ifndef SHADERCONTENTLOADER_H
#define SHADERCONTENTLOADER_H

#include "ResourceLoader.h"

class ShaderContentLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
protected:
	void ProcessInclude (const std::string& line, std::string& content, const std::string& filename);
};

#endif