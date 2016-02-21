#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <map>
#include <string>

#include "Material/Material.h"

class MaterialManager
{
private:
	Material* _default;
	std::map<std::string, Material*> _materials;
public:
	Material* Default ();
	~MaterialManager ();

	static MaterialManager& Instance ();

	void AddMaterial (Material* material);
	Material* GetMaterial (std::string name);
private:
	MaterialManager ();
};

#endif