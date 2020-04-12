#ifndef PROJECTPROCESSOR_H
#define PROJECTPROCESSOR_H

#include <map>
#include <experimental/filesystem>

#include "HeaderData.h"
#include "HeaderProcessor.h"

namespace fs = std::experimental::filesystem;

struct ProjectData
{
	std::map<std::string, ClassType*> Data;
};

class ProjectProcessor
{
protected:
	ProjectData _project;

public:
	ProjectData Process (const std::string& path);
protected:
	void ProcessDirectory (const fs::path fspath);
	void AttachHeader (const ClassProduct& classProduct, const std::string& path);
};

#endif