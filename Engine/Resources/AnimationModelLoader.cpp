#include "AnimationModelLoader.h"

#include "Mesh/AnimationNode.h"

#include "Core/Console/Console.h"

#include "Resources.h"

#include "Mesh/VertexBoneInfo.h"
#include "Mesh/BoneTree.h"

#include "Utils/Conversions/Matrices.h"
#include "Utils/Conversions/Quaternions.h"
#include "Utils/Conversions/Vectors.h"

Object* AnimationModelLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		Console::LogError (filename + " has error in its syntax. Could not preceed further.");
		return nullptr;
	}

	TiXmlElement* root = doc.FirstChildElement ("AnimatedModel");

	if (root == nullptr) {
		return nullptr;
	}

	AnimationModel* animModel = GetAnimationModel (root);

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Animation") {
			ProcessAnimation (content, animModel->GetAnimationsController ());
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return animModel;
}

AnimationModel* AnimationModelLoader::GetAnimationModel (TiXmlElement* xmlElem)
{
	std::string skinPath = xmlElem->Attribute ("skinPath");

	AnimationModel* animModel = Resources::LoadSkinModel (skinPath);

	AnimationsController* animController = new AnimationsController ();
	animModel->SetAnimationsController (animController);

	return animModel;
}

void AnimationModelLoader::ProcessAnimation (TiXmlElement* xmlElem, AnimationsController* animController)
{
	std::string animationName = xmlElem->Attribute ("name");
	std::string animationPath = xmlElem->Attribute ("path");

	AnimationContainer* animContainer = Resources::LoadAnimationClip (animationPath);

	animContainer->SetName (animationName);

	animController->AddAnimationContainer (animContainer);
}
