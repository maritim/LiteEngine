#ifndef PROJECTGENERATOR_H
#define PROJECTGENERATOR_H

#include <string>
#include <filesystem>

#include "HeaderData.h"

namespace fs = std::filesystem;

class ProjectGenerator
{
public:
	void Generate (const std::string& path, const std::string& generatedPath, const std::string& generatedAPI);
protected:
	std::string ProcessHeader (const ClassType* classType, const std::string& generatedAPI);

	bool IsComponent (const ClassType* classType);
};

#endif