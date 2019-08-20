#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <string>

#include "Core/Resources/Resource.h"
#include "Material.h"

class MaterialLibrary : public Object
{
private:
	std::string _name;
	std::vector<Resource<Material>> _materials;
public:
	MaterialLibrary();
	~MaterialLibrary();

	void SetName (const std::string& name);
	void AddMaterial(const Resource<Material>& material);

	std::string GetName () const;
	std::size_t GetMaterialsCount() const;
	Resource<Material> GetMaterial(std::size_t index) const;
	Resource<Material> GetMaterial (std::string name) const;

	void Clear();
};

#endif