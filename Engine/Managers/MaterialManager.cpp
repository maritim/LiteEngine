#include "MaterialManager.h"

#include <map>
#include <string>

#include "Resources/Resources.h"

#include "Material/MaterialLibrary.h"

MaterialManager::MaterialManager () :
	_materials ()
{

}

MaterialManager::~MaterialManager ()
{
	std::map<std::string, Material*>::iterator it;
	for (it = _materials.begin(); it != _materials.end(); ++it) {
		delete it->second;
	}
}

SPECIALIZE_SINGLETON(MaterialManager)

void MaterialManager::AddMaterial (Material* material)
{
	if (material == nullptr) {
		return ;
	}

	if (_materials.find (material->name) != _materials.end ()) {
		return ;
	}

	_materials [material->name] = material;
}

Material* MaterialManager::GetMaterial (const std::string& name)
{
	if (_materials.find (name) == _materials.end ()) {
		return nullptr;
	}

	return _materials [name];
}