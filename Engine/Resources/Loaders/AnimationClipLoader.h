#ifndef ANIMATIONCLIPLOADER_H
#define ANIMATIONCLIPLOADER_H

#include "Resources/ResourceLoader.h"

#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Mesh/AnimationContainer.h"

class AnimationClipLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	void ProcessAnimation (AnimationContainer* animContainer, const aiScene* assimpScene);
	void ProcessAnimationNode (AnimationContainer* animContainer, aiNodeAnim* assimpAnimNode);
};

#endif