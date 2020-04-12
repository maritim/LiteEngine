#ifndef HEADERDATA_H
#define HEADERDATA_H

#include <string>
#include <vector>

struct AttributeType;

struct ClassType
{
	std::string TypeName;
	std::vector<ClassType*> Parents;
	std::vector<AttributeType> Attributes;

	std::vector<ClassType*> Children;

	std::string Path;
	bool IsActive;
};

struct AttributeType
{
	std::string TypeName;
	std::string Name;
};

#endif