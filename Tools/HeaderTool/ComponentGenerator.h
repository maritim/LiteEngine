#ifndef COMPONENTGENERATOR_H
#define COMPONENTGENERATOR_H

#include "HeaderData.h"

class ComponentGenerator
{
public:
	std::string Generate (const ClassType* classType, const std::string& generatedAPI);
protected:
	std::string GenerateLoader (const ClassType* classType);
	std::string GenerateSaver (const ClassType* classType);
	std::string GenerateWidget (const ClassType* classType);

	std::string GenerateAttributesLoader (const ClassType* classType);
	std::string GenerateAttributesSaver (const ClassType* classType);
	std::string GenerateAttributesWidget (const ClassType* classType);
};

#endif