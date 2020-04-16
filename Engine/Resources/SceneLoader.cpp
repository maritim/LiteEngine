#include "SceneLoader.h"

#include <string>

#include "Core/Resources/Resource.h"

#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "Skybox/Skybox.h"

#include "Systems/Components/Component.h"
#include "Systems/Components/PersistentComponent.h"

#include "Resources/Resources.h"

#include "Utils/Extensions/StringExtend.h"
#include "Utils/Extensions/MathExtend.h"

#include "Core/Console/Console.h"

SceneLoader::SceneLoader ()
{

}

SceneLoader& SceneLoader::Instance ()
{
	static SceneLoader sceneLoader;

	return sceneLoader;
}

Scene* SceneLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		Console::LogError (filename + " has error in its syntax. Could not preceed further.");
		return NULL;
	}

	TiXmlElement* root = doc.FirstChildElement ("Scene");

	if (root == NULL) {
		return NULL;
	}

	Scene* scene = new Scene ();

	scene->SetPath (filename);

	std::string name = root->Attribute ("name");
	scene->SetName (name);

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Skybox") {
			ProcessSkybox (content, scene);
		}
		else if (name == "SceneObject") {
			ProcessSceneObject (content, scene);
		}
		else if (name == "ParticleSystem") {
			ProcessParticleSystem (content, scene);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return scene;
}

void SceneLoader::ProcessSkybox (TiXmlElement* xmlElem, Scene* scene)
{
	std::string skyboxPath = xmlElem->Attribute ("path");

	Skybox* skybox = Resources::LoadSkybox (skyboxPath);

	scene->SetSkybox (skybox);
}

void SceneLoader::ProcessSceneObject (TiXmlElement* xmlElem, Scene* scene)
{
	std::string name = xmlElem->Attribute ("name");
	std::string instanceID = xmlElem->Attribute ("instanceID");
	std::string isActive = xmlElem->Attribute ("isActive");

	SceneObject* sceneObject = new SceneObject ();
	sceneObject->SetName (name);
	// Need unsigned int here
	sceneObject->SetInstanceID (std::stoi (instanceID));
	sceneObject->SetActive (Extensions::StringExtend::ToBool (isActive));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Transform") {
			ProcessTransform (content, scene, sceneObject);
		}
		else if (name == "Components") {
			ProcessComponents (content, sceneObject);
		}

		content = content->NextSiblingElement ();
	}

	scene->AttachObject (sceneObject);
}

void SceneLoader::ProcessParticleSystem (TiXmlElement* xmlElem, Scene* scene)
{
	std::string name = xmlElem->Attribute ("name");
	std::string instanceID = xmlElem->Attribute ("InstanceID");
	std::string path = xmlElem->Attribute ("path");
	std::string isActive = xmlElem->Attribute ("isActive");

	ParticleSystem* partSystem = Resources::LoadParticleSystem (path);
	partSystem->SetName (name);
	// Need unsigned int here
	partSystem->SetInstanceID (std::stoi (instanceID));
	partSystem->SetActive (Extensions::StringExtend::ToBool (isActive));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Transform") {
			ProcessTransform (content, scene, partSystem);
		}

		content = content->NextSiblingElement ();
	}

	scene->AttachObject (partSystem);
}

void SceneLoader::ProcessTransform (TiXmlElement* xmlElem, Scene* scene, SceneObject* sceneObject)
{
	Transform* transform = sceneObject->GetTransform ();

	TiXmlElement* content = xmlElem->FirstChildElement ();

	const char* parentID = xmlElem->Attribute ("parentID");
	if (parentID != NULL) {
		SceneObject* parent =  scene->GetObject (std::stoi (parentID));

		transform->SetParent (parent->GetTransform ());
	}

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Position") {
			transform->SetPosition (GetPosition (content));
		}
		else if (name == "Rotation") {
			transform->SetRotation (GetRotation (content));
		}
		else if (name == "Scale") {
			transform->SetScale (GetScale (content));
		}

		content = content->NextSiblingElement ();
	}
}

glm::vec3 SceneLoader::GetPosition (TiXmlElement* xmlElem)
{
	glm::vec3 vector3 (0.0f);

	const char* x = xmlElem->Attribute ("x");
	const char* y = xmlElem->Attribute ("y");
	const char* z = xmlElem->Attribute ("z");

	if (x) {
		vector3.x = std::stof (x);
	}

	if (y) {
		vector3.y = std::stof (y);
	}

	if (z) {
		vector3.z = std::stof (z);
	}

	return vector3;
}

glm::quat SceneLoader::GetRotation (TiXmlElement* xmlElem)
{
	/*
	 * The rotation data extracted from scene exists as axis angles in degrees.
	 * It needs to be converted into euler angles by converting the degrees in
	 * radians.
	*/

	glm::vec3 eulerAngles = GetPosition (xmlElem) * DEG2RAD;

	/*
	 * The euler angles are converted in quaternions, in which type will be
	 * processed from now on. The specialised glm::quat constructor is used
	 * for conversion.
	*/

	return glm::quat (eulerAngles);
}

glm::vec3 SceneLoader::GetScale (TiXmlElement* xmlElem)
{
	glm::vec3 vector3 (1.0f);

	const char* x = xmlElem->Attribute ("x");
	const char* y = xmlElem->Attribute ("y");
	const char* z = xmlElem->Attribute ("z");

	if (x) {
		vector3.x = std::stof (x);
	}

	if (y) {
		vector3.y = std::stof (y);
	}

	if (z) {
		vector3.z = std::stof (z);
	}

	return vector3;
}

void SceneLoader::ProcessComponents (TiXmlElement* xmlElem, SceneObject* sceneObject)
{
	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Component") {
			ProcessComponent (content, sceneObject);
		}

		content = content->NextSiblingElement ();
	}
}

void SceneLoader::ProcessComponent (TiXmlElement* xmlElem, SceneObject* sceneObject)
{
	std::string name = xmlElem->Attribute ("name");

	if (name == std::string ()) {
		return;
	}

	Component* component = ObjectsFactory<Component>::Instance ()->Create ("HT" + name);

	if (component == nullptr) {
		return;
	}

	PersistentComponent* persistentComponent = dynamic_cast<PersistentComponent*> (component);
	persistentComponent->Load (xmlElem);

	sceneObject->AttachComponent (component);
}
