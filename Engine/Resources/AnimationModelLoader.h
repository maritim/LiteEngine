#ifndef ANIMATIONMODELLOADER_H
#define ANIMATIONMODELLOADER_H

#include "GenericObjectModelLoader.h"

#include "Mesh/AnimationModel.h"
#include "Mesh/AnimationsController.h"
#include "Mesh/AnimationContainer.h"

#include "Mesh/BoneNode.h"

class AnimationModelLoader : public GenericObjectModelLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	void ProcessTree (AnimationModel* model, aiNode* node, const aiScene* assimpScene, const std::string& filename);

	void ProcessObject (AnimationModel* model, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename);

	void ProcessAnimations (AnimationModel* model, const aiScene* assimpScene);
	void ProcessAnimation (AnimationsController* animController, aiAnimation* assimpAnimation);
	void ProcessAnimationNode (AnimationContainer* animContainer, aiNodeAnim* assimpAnimNode);

	void ProcessBones (AnimationModel* animModel, const aiMesh* assimpMesh);

	void ProcessAnimationTree (AnimationModel* animModel, aiNode* assimpNode);
	void ProcessAnimationNode (BoneNode*& node, aiNode* assimpNode);
};

#endif