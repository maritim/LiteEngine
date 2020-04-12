#ifndef PROJECTGENERATOR_H
#define PROJECTGENERATOR_H

#include <string>
#include <experimental/filesystem>

#include "HeaderData.h"

namespace fs = std::experimental::filesystem;

class ProjectGenerator
{
public:
	void Generate (const std::string& path, const std::string& generatedPath);
protected:
	std::string ProcessHeader (const ClassType* classType);

	bool IsComponent (const ClassType* classType);
};

#endif