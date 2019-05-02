#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "Mesh/Model.h"

class ModelManager : public Singleton<ModelManager>
{
	friend Singleton<ModelManager>;

private:
	std::map<std::string, Model*> _models;

public:
	void AddModel (Model* model);
	Model* GetModel (const std::string& filename);
private:
	ModelManager ();
	~ModelManager ();
	ModelManager (const ModelManager&);
	ModelManager& operator=(const ModelManager&);
};

#endif