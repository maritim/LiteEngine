#ifndef PROJECTPROCESSOR_H
#define PROJECTPROCESSOR_H

#include <map>
#include <filesystem>

#include "HeaderData.h"
#include "HeaderProcessor.h"

namespace fs = std::filesystem;

struct ProjectData
{
	std::map<std::string, ClassType*> Data;
};

class ProjectProcessor
{
protected:
	ProjectData _project;

public:
	ProjectData Process (const std::string& path, const std::string& generatedAPI);
protected:
	void ProcessDirectory (const fs::path fspath, const std::string& generatedAPI);
	void AttachHeader (const ClassProduct& classProduct, const std::string& path);
};

#endif