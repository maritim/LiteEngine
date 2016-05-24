#include "AnimationModelLoader.h"

#include "Mesh/AnimationNode.h"

#include "Core/Console/Console.h"
#include "Core/Debug/Debug.h"

#include "Mesh/VertexBoneInfo.h"
#include "Mesh/BoneTree.h"

#include "Utils/Conversions/Matrices.h"
#include "Utils/Conversions/Quaternions.h"
#include "Utils/Conversions/Vectors.h"

Object* AnimationModelLoader::Load (const std::string& fileName)
{
	AnimationModel* animModel = new AnimationModel ();

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

	if (assimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || assimpScene->mRootNode == nullptr) {
		Console::LogError ("Unable to scuccessfully process \"" + fileName + "\" model!");

		exit (0);
	}

	ProcessTree (animModel, assimpScene->mRootNode, assimpScene, fileName);
	ProcessAnimations (animModel, assimpScene);
	ProcessAnimationTree (animModel, assimpScene->mRootNode);

	return animModel;
}

void AnimationModelLoader::ProcessObject (AnimationModel* model, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename)
{
	ObjectModel* objectModel = new ObjectModel (assimpMesh->mName.data);

	ProcessFaces (model, objectModel, assimpMesh, assimpScene, filename);

	ProcessBones (model, assimpMesh);

	ProcessVertices (model, assimpMesh);
	ProcessNormals (model, assimpMesh);
	ProcessTexcoords (model, assimpMesh);

	model->AddObjectModel (objectModel);
}

void AnimationModelLoader::ProcessTree (AnimationModel* animModel, aiNode* assimpNode, const aiScene* assimpScene, const std::string& filename)
{
	for (std::size_t i=0;i<assimpNode->mNumMeshes;i++) {
		aiMesh* assimpMesh = assimpScene->mMeshes [assimpNode->mMeshes [i]];
		ProcessObject (animModel, assimpMesh, assimpScene, filename);
	}

	for (std::size_t i=0;i<assimpNode->mNumChildren;i++) {
		ProcessTree (animModel, assimpNode->mChildren [i], assimpScene, filename);
	}
}

void AnimationModelLoader::ProcessAnimationTree (AnimationModel* animModel, aiNode* assimpNode)
{
	BoneTree* boneTree = new BoneTree ();

	BoneNode* boneNode = new BoneNode ();
	ProcessAnimationNode (boneNode, assimpNode);
	boneTree->SetRoot (boneNode);

	animModel->SetBoneTree (boneTree);
}

void AnimationModelLoader::ProcessAnimationNode (BoneNode*& boneNode, aiNode* assimpNode)
{
	boneNode->SetName (assimpNode->mName.data);

	glm::mat4 transformMatrix = Conversions::AssimpMatrixToGLMMat (assimpNode->mTransformation);
	boneNode->SetTransform (transformMatrix);

	for (std::size_t i=0;i<assimpNode->mNumChildren;i++) {
		BoneNode* bone = new BoneNode (boneNode);

		ProcessAnimationNode (bone, assimpNode->mChildren [i]);

		boneNode->AddChild (bone);
	}
}

void AnimationModelLoader::ProcessAnimations (AnimationModel* animModel, const aiScene* assimpScene)
{
	AnimationsController* animController = new AnimationsController ();

	for (std::size_t i=0;i<assimpScene->mNumAnimations;i++) {
		aiAnimation* assimpAnimation = assimpScene->mAnimations [i];

		ProcessAnimation (animController, assimpAnimation);
	}

	animModel->SetAnimationsController (animController);
}

void AnimationModelLoader::ProcessAnimation (AnimationsController* animController, aiAnimation* assimpAnimation)
{
	AnimationContainer* animContainer = new AnimationContainer ();

	Console::LogWarning (assimpAnimation->mName.data);

	animContainer->SetName (assimpAnimation->mName.data);
	animContainer->SetDuration (assimpAnimation->mDuration);
	animContainer->SetTicksPerSecond (assimpAnimation->mTicksPerSecond);

	for (std::size_t i=0;i<assimpAnimation->mNumChannels;i++) {
		aiNodeAnim* assimpAnimNode = assimpAnimation->mChannels [i];

		ProcessAnimationNode (animContainer, assimpAnimNode);
	}

	animController->AddAnimationContainer (animContainer);
}

void AnimationModelLoader::ProcessAnimationNode (AnimationContainer* animContainer, aiNodeAnim* assimpAnimNode)
{
	AnimationNode* animNode = new AnimationNode ();

	std::string boneName = assimpAnimNode->mNodeName.data;

	for (std::size_t i=0;i<assimpAnimNode->mNumRotationKeys;i++) {
		QuatKey rotationKey;
		rotationKey.value = Conversions::AssimpQuaternionToGLMQuat (assimpAnimNode->mRotationKeys [i].mValue);
		rotationKey.time = assimpAnimNode->mRotationKeys [i].mTime;

		animNode->AddRotationKey (rotationKey);
	}

	for (std::size_t i=0;i<assimpAnimNode->mNumScalingKeys;i++) {
		VectorKey scalingKey;
		scalingKey.value = Conversions::AssimpVectorToGLMVec (assimpAnimNode->mScalingKeys [i].mValue);
		scalingKey.time = assimpAnimNode->mScalingKeys [i].mTime;

		animNode->AddScalingKey (scalingKey);
	}

	for (std::size_t i=0;i<assimpAnimNode->mNumPositionKeys;i++) {
		VectorKey positionKey;
		positionKey.value = Conversions::AssimpVectorToGLMVec (assimpAnimNode->mPositionKeys [i].mValue);
		positionKey.time = assimpAnimNode->mPositionKeys [i].mTime;

		animNode->AddPositionKey (positionKey);
	}

	animContainer->AddAnimationNode (boneName, animNode);
}

void AnimationModelLoader::ProcessBones (AnimationModel* model, const aiMesh* assimpMesh)
{
	for (std::size_t i=0;i<assimpMesh->mNumBones;i++) {
		std::string boneName = assimpMesh->mBones [i]->mName.data;
		std::size_t boneID = 0;

		if (model->GetBone (boneName) == nullptr) {
			BoneInfo* bone = new BoneInfo ();
			bone->SetName (boneName);

			glm::mat4 offsetMatrix = Conversions::AssimpMatrixToGLMMat (assimpMesh->mBones [i]->mOffsetMatrix);
			bone->SetTransformMatrix (offsetMatrix);

			model->AddBone (bone);
		
			boneID = bone->GetID ();
		} else {
			boneID = model->GetBone (boneName)->GetID ();
		}

		for (std::size_t j=0;j<assimpMesh->mBones [i]->mNumWeights;j++) {
			std::size_t vertexID = model->VertexCount () + assimpMesh->mBones [i]->mWeights [j].mVertexId;
			float weight = assimpMesh->mBones [i]->mWeights [j].mWeight;

			model->GetVertexBoneInfo (vertexID)->AddBone (boneID, weight);
		}
	}
}
