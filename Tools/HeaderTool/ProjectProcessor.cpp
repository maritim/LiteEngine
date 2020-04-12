#include "ProjectProcessor.h"

ProjectData ProjectProcessor::Process (const std::string& path)
{
	fs::path fspath (path);

	ProcessDirectory (fspath);

	return _project;
}

void ProjectProcessor::ProcessDirectory (const fs::path fspath)
{
	for (const auto& entry : fs::directory_iterator(fspath)) {
		if (fs::is_directory (entry.status ())) {
			ProcessDirectory (entry.path ());

			continue;
		}

		if (entry.path ().extension () == ".h"
			|| entry.path ().extension () == ".hpp") {

			HeaderProcessor headerProcessor;

			auto classProduct = headerProcessor.Process (entry.path ().string ());

			AttachHeader (classProduct, entry.path().string ());
		}
	}
}

void ProjectProcessor::AttachHeader (const ClassProduct& classProduct, const std::string& path)
{
	ClassType* classType = nullptr;

	/*
	 * Attach header to project
	*/

	auto it = _project.Data.find (classProduct.TypeName);

	if (it == _project.Data.end ()) {
		ClassType* classType = new ClassType ();

		classType->TypeName = classProduct.TypeName;

		_project.Data [classProduct.TypeName] = classType;
	}

	classType = _project.Data [classProduct.TypeName];

	/*
	 * 
	*/

	for (auto parentName : classProduct.ParentNames) {
		auto it = _project.Data.find (parentName);

		if (it == _project.Data.end ()) {

			ClassType* parentType = new ClassType ();
			parentType->TypeName = parentName;
			parentType->IsActive = false;

			_project.Data [parentName] = parentType;
		}

		_project.Data [parentName]->Children.push_back (classType);
		classType->Parents.push_back (_project.Data [parentName]);
	}

	for (auto& attribute : classProduct.Attributes) {
		AttributeType attributeType;

		attributeType.TypeName = attribute.TypeName;
		attributeType.Name = attribute.Name;

		classType->Attributes.push_back (attributeType);
	}

	classType->Path = path;
	classType->IsActive = true;
}
