#include "MaterialLibrary.h"

MaterialLibrary::MaterialLibrary()
{

}

MaterialLibrary::~MaterialLibrary()
{

}

void MaterialLibrary::SetName (const std::string& name)
{
	_name = name;
}

void MaterialLibrary::AddMaterial(const Resource<Material>& material)
{
	_materials.push_back (material);
}

std::string MaterialLibrary::GetName () const
{
	return _name;
}

std::size_t MaterialLibrary::GetMaterialsCount () const
{
	return _materials.size();
}

Resource<Material> MaterialLibrary::GetMaterial(std::size_t index) const
{
	if (index >= _materials.size()) {
		return NULL;
	}
	return _materials[index];
}

Resource<Material> MaterialLibrary::GetMaterial (std::string matName) const
{
	for (std::size_t i=0;i<_materials.size ();i++) {
		if (_materials [i]->name == matName) {
			return _materials [i];
		}
	}

	return NULL;
}

void MaterialLibrary::Clear()
{

}