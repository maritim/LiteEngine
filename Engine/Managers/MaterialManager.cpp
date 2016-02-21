#include "MaterialManager.h"

#include <map>
#include <string>

#include "Resources/Resources.h"

#include "Material/MaterialLibrary.h"

MaterialManager::MaterialManager ()
{
	char defaultMaterial[] = "Assets/Materials/default.mtl";

	MaterialLibrary* materialLibrary = Resources::LoadMaterialLibrary (defaultMaterial);

	_default = materialLibrary->GetMaterial (0);

	for (std::size_t i=1;i<materialLibrary->GetMaterialsCount ();i++) {
		delete materialLibrary->GetMaterial (i);
	}

	delete materialLibrary;
}

MaterialManager& MaterialManager::Instance ()
{
	static MaterialManager instance;

	return instance;
}

Material* MaterialManager::Default ()
{
	return _default;
}

void MaterialManager::AddMaterial (Material* material)
{
	if (!material) {
		return ;
	}

	if (_materials.find (material->name) != _materials.end ()) {
		return ;
	}

	_materials [material->name] = material;
}

Material* MaterialManager::GetMaterial (std::string name)
{
	if (_materials.find (name) == _materials.end ()) {
		return NULL;
	}

	return _materials [name];
}

MaterialManager::~MaterialManager ()
{
	delete _default;

	std::map<std::string, Material*>::iterator it;
	for (it = _materials.begin(); it != _materials.end(); ++it) {
		delete it->second;
	}
}