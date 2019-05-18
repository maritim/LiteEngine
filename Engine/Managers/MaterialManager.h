#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "Material/Material.h"

class ENGINE_API MaterialManager : public Singleton<MaterialManager>
{
	friend Singleton<MaterialManager>;

	DECLARE_SINGLETON(MaterialManager)

private:
	std::map<std::string, Material*> _materials;

public:
	void AddMaterial (Material* material);
	Material* GetMaterial (const std::string& name);
private:
	MaterialManager ();
	~MaterialManager ();
	MaterialManager (const MaterialManager&);
	MaterialManager& operator=(const MaterialManager&);
};

#endif