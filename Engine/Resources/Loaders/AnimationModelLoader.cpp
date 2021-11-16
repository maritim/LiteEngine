#include "AnimationModelLoader.h"

#include "Renderer/Render/Mesh/AnimationNode.h"

#include "Core/Console/Console.h"

#include "Resources/Resources.h"

#include "Utils/Files/FileSystem.h"

using namespace tinyxml2;

Object* AnimationModelLoader::Load (const std::string& filename)
{
	XMLDocument doc;
	if(doc.LoadFile(filename.c_str ()) != XML_SUCCESS) {
		Console::LogError (filename + " has error(s) in its syntax. Cannot proceed further.");
		return nullptr;
	}

	XMLElement* root = doc.FirstChildElement ("AnimatedModel");

	if (root == nullptr) {
		return nullptr;
	}

	AnimationModel* animModel = GetAnimationModel (root, filename);

	XMLElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Animation") {
			ProcessAnimation (content, animModel->GetAnimationsController (), filename);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return animModel;
}

AnimationModel* AnimationModelLoader::GetAnimationModel (XMLElement* xmlElem, const std::string& filename)
{
	std::string skinPath = xmlElem->Attribute ("skinPath");

	std::string directory = FileSystem::GetDirectory (filename);
	skinPath = directory + skinPath;

	AnimationModel* animModel = Resources::LoadSkinModel (skinPath);

	AnimationsController* animController = new AnimationsController ();
	animModel->SetAnimationsController (animController);

	return animModel;
}

void AnimationModelLoader::ProcessAnimation (XMLElement* xmlElem, AnimationsController* animController, const std::string& filename)
{
	std::string animationName = xmlElem->Attribute ("name");
	std::string animationPath = xmlElem->Attribute ("path");

	std::string directory = FileSystem::GetDirectory (filename);
	animationPath = directory + animationPath;

	AnimationContainer* animContainer = Resources::LoadAnimationClip (animationPath);

	animContainer->SetName (animationName);

	animController->AddAnimationContainer (animContainer);
}
