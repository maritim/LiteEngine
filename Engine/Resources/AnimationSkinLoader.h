#ifndef ANIMATIONSKINLOADER_H
#define ANIMATIONSKINLOADER_H

#include "GenericObjectModelLoader.h"

#include "Mesh/AnimationModel.h"

#include "Mesh/BoneNode.h"

class AnimationSkinLoader : public GenericObjectModelLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	void ProcessTree (AnimationModel* model, aiNode* node, const aiScene* assimpScene, const std::string& filename);

	void ProcessObject (AnimationModel* model, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename);

	void ProcessBones (AnimationModel* animModel, const aiMesh* assimpMesh);

	void ProcessBoneTree (AnimationModel* animModel, aiNode* assimpNode);
	void ProcessBoneNode (BoneNode*& node, aiNode* assimpNode);
};

#endif