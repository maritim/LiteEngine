#include "ModelManager.h"

ModelManager::ModelManager () :
	_models ()
{

}

ModelManager::~ModelManager ()
{

}

SPECIALIZE_SINGLETON(ModelManager)

void ModelManager::AddModel (Model* model)
{
	if (_models.find (model->GetName()) != _models.end ()) {
		return ;
	}

	_models [model->GetName()] = model;
}

Model* ModelManager::GetModel (const std::string& filename)
{
	if (_models.find (filename) == _models.end ()) {
		return nullptr;
	}

	return _models [filename];
}