#include "MaterialLibrary.h"

MaterialLibrary::MaterialLibrary()
{

}

MaterialLibrary::~MaterialLibrary()
{

}

void MaterialLibrary::AddMaterial(Material* material)
{
	_materials.push_back (material);
}

std::size_t MaterialLibrary::GetMaterialsCount () const
{
	return _materials.size();
}

Material* MaterialLibrary::GetMaterial(std::size_t index) const
{
	if (index >= _materials.size()) {
		return NULL;
	}
	return _materials[index];
}

Material* MaterialLibrary::GetMaterial (std::string matName) const
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
	for (std::size_t i=0;i<_materials.size();i++) {
		delete _materials [i];
	}
}