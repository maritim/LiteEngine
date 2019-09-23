#ifndef GENERICOBJECTMODELLOADER_H
#define GENERICOBJECTMODELLOADER_H

#include "Resources/ResourceLoader.h"

#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Core/Interfaces/Object.h"

#include "Mesh/Model.h"
#include "Mesh/ObjectModel.h"
#include "Mesh/PolygonGroup.h"
#include "Mesh/Polygon.h"

class GenericObjectModelLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& fileName);
protected:
	void ProcessTree (Model* model, aiNode* node, const aiScene* assimpScene, const std::string& filename);

	void ProcessVertices (Model* model, aiMesh* assimpMesh);
	void ProcessNormals (Model* model, aiMesh* assimpMesh);
	void ProcessTexcoords (Model* model, aiMesh* assimpMesh);

	void ProcessFaces (Model* model, ObjectModel* objectModel, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename);

	void ProcessObject (Model* model, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename);

	void ProcessMaterial (PolygonGroup* group, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename);
};

#endif