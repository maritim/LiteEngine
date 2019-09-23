#include "AnimationClipLoader.h"

#include "Mesh/AnimationNode.h"

#include "Core/Console/Console.h"

#include "Mesh/VertexBoneInfo.h"
#include "Mesh/BoneTree.h"

#include "Utils/Conversions/Matrices.h"
#include "Utils/Conversions/Quaternions.h"
#include "Utils/Conversions/Vectors.h"

Object* AnimationClipLoader::Load (const std::string& fileName)
{
	AnimationContainer* animContainer = new AnimationContainer ();

	Assimp::Importer assimpImporter;

	const aiScene* assimpScene = assimpImporter.ReadFile (fileName, 
		aiProcess_GenSmoothNormals | 
		aiProcess_Triangulate | 
		aiProcess_FlipUVs |
		aiProcess_LimitBoneWeights);

	if (assimpScene == nullptr) {
		Console::LogError ("Unable to load \"" + fileName + "\" model!");

		exit (0);
	}

	if (assimpScene->mRootNode == nullptr) {
		Console::LogError ("Unable to scuccessfully process \"" + fileName + "\" model!");

		exit (0);
	}

	ProcessAnimation (animContainer, assimpScene);

	return animContainer;
}

void AnimationClipLoader::ProcessAnimation (AnimationContainer* animContainer, const aiScene* assimpScene)
{
	aiAnimation* assimpAnimation = assimpScene->mAnimations [0];

	animContainer->SetName (assimpAnimation->mName.data);
	animContainer->SetDuration ((float) assimpAnimation->mDuration);
	animContainer->SetTicksPerSecond ((float) assimpAnimation->mTicksPerSecond);

	for (std::size_t i=0;i<assimpAnimation->mNumChannels;i++) {
		aiNodeAnim* assimpAnimNode = assimpAnimation->mChannels [i];

		ProcessAnimationNode (animContainer, assimpAnimNode);
	}
}

void AnimationClipLoader::ProcessAnimationNode (AnimationContainer* animContainer, aiNodeAnim* assimpAnimNode)
{
	AnimationNode* animNode = new AnimationNode ();

	std::string boneName = assimpAnimNode->mNodeName.data;

	for (std::size_t i=0;i<assimpAnimNode->mNumRotationKeys;i++) {
		QuatKey rotationKey;
		rotationKey.value = Conversions::AssimpQuaternionToGLMQuat (assimpAnimNode->mRotationKeys [i].mValue);
		rotationKey.time = (float) assimpAnimNode->mRotationKeys [i].mTime;

		animNode->AddRotationKey (rotationKey);
	}

	for (std::size_t i=0;i<assimpAnimNode->mNumScalingKeys;i++) {
		VectorKey scalingKey;
		scalingKey.value = Conversions::AssimpVectorToGLMVec (assimpAnimNode->mScalingKeys [i].mValue);
		scalingKey.time = (float) assimpAnimNode->mScalingKeys [i].mTime;

		animNode->AddScalingKey (scalingKey);
	}

	for (std::size_t i=0;i<assimpAnimNode->mNumPositionKeys;i++) {
		VectorKey positionKey;
		positionKey.value = Conversions::AssimpVectorToGLMVec (assimpAnimNode->mPositionKeys [i].mValue);
		positionKey.time = (float) assimpAnimNode->mPositionKeys [i].mTime;

		animNode->AddPositionKey (positionKey);
	}

	animContainer->AddAnimationNode (boneName, animNode);
}
