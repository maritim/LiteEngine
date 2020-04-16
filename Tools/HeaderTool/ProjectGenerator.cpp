#include "ProjectGenerator.h"

#include <fstream>

#include "ProjectProcessor.h"
#include "ComponentGenerator.h"

void ProjectGenerator::Generate (const std::string& path, const std::string& generatedPath)
{
	std::string source;

	ProjectProcessor projectProcessor;

	auto project = projectProcessor.Process (path);

	source = "#include \"Systems/Components/EditorComponent.h\"\n"
			 "#include \"Resources/Loaders/ComponentLoaders/ComponentAttributeLoader.h\"\n"
			 "#include \"Resources/Savers/ComponentSavers/ComponentAttributeSaver.h\"\n"
			 "#include \"Systems/Components/ComponentAttributeWidget.h\"\n\n";

	for (auto classType : project.Data) {
		source += ProcessHeader (classType.second);
	}

	std::ofstream f(generatedPath);

	f << source;

	f.close ();
}

std::string ProjectGenerator::ProcessHeader (const ClassType* classType)
{
	std::string source;

	if (classType->IsActive == true && IsComponent (classType)) {

		ComponentGenerator componentGenerator;

		source += componentGenerator.Generate (classType);
	}

	return source;
}

bool ProjectGenerator::IsComponent (const ClassType* classType)
{
	if (classType->TypeName == "Component") {
		return true;
	}

	for (auto parentType : classType->Parents) {
		if (IsComponent (parentType)) {
			return true;
		}
	}

	return false;
}
