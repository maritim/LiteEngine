#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <string>

#include "Material.h"

class MaterialLibrary : public Object
{
private:
	std::vector<Material*> _materials;
public:
	MaterialLibrary();
	~MaterialLibrary();

	void AddMaterial(Material* material);

	std::size_t GetMaterialsCount() const;
	Material* GetMaterial(std::size_t index) const;
	Material* GetMaterial (std::string name) const;

	void Clear();
};

#endif