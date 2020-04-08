#include "SceneLoader.h"

#include <string>

#include "Core/Resources/Resource.h"

#include "SceneNodes/GameObject.h"
#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "Mesh/Model.h"
#include "Skybox/Skybox.h"

#include "Systems/Components/Component.h"

#include "Resources/Resources.h"
#include "Resources/ComponentLoader.h"

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

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Light") {
			ProcessLight (content, scene);
		}
		else if (name == "Skybox") {
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
	std::string instanceID = xmlElem->Attribute ("InstanceID");
	std::string isActive = xmlElem->Attribute ("isActive");

	GameObject* gameObject = new GameObject ();
	gameObject->SetName (name);
	// Need unsigned int here
	gameObject->SetInstanceID (std::stoi (instanceID));
	gameObject->SetActive (Extensions::StringExtend::ToBool (isActive));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Transform") {
			ProcessTransform (content, scene, gameObject);
		}
		else if (name == "Rigidbody") {
			ProcessRigidbody (content, gameObject);
		}
		else if (name == "AudioSource") {
			ProcessAudioSource (content, gameObject);
		}
		else if (name == "Components") {
			ProcessComponents (content, gameObject);
		}

		content = content->NextSiblingElement ();
	}

	scene->AttachObject (gameObject);
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

void SceneLoader::ProcessLight (TiXmlElement* xmlElem, Scene* scene)
{
	std::string name = xmlElem->Attribute ("name");
	std::string instanceID = xmlElem->Attribute ("InstanceID");
	std::string path = xmlElem->Attribute ("path");
	std::string isActive = xmlElem->Attribute ("isActive");

	Light* light = Resources::LoadLight (path);
	light->SetName (name);
	light->SetInstanceID (std::stoi (instanceID));
	light->SetActive (Extensions::StringExtend::ToBool (isActive));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Transform") {
			ProcessTransform (content, scene, light);
		}

		content = content->NextSiblingElement ();
	}

	scene->AttachObject (light);
}

void SceneLoader::ProcessTransform (TiXmlElement* xmlElem, Scene* scene, SceneObject* sceneObject)
{
	Transform* transform = sceneObject->GetTransform ();

	TiXmlElement* content = xmlElem->FirstChildElement ();

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

	const char* parentName = xmlElem->Attribute ("parent");
	if (parentName != NULL) {
		SceneObject* parent =  scene->GetObject (parentName);

		transform->SetParent (parent->GetTransform ());
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

void SceneLoader::ProcessComponents (TiXmlElement* xmlElem, GameObject* gameObject)
{
	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Component") {
			ProcessComponent (content, gameObject);
		}

		content = content->NextSiblingElement ();
	}
}

void SceneLoader::ProcessComponent (TiXmlElement* xmlElem, GameObject* gameObject)
{
	std::string name = xmlElem->Attribute ("name");

	if (name == std::string ()) {
		return;
	}

	ComponentLoader* componentLoader = ObjectsFactory<ComponentLoader>::Instance ()->Create (name + "Loader");

	if (componentLoader == nullptr) {
		return;
	}

	Component* component = componentLoader->Load (xmlElem);

	delete componentLoader;

	if (component == nullptr) {
		return;
	}

	gameObject->AttachComponent (component);	
}

void SceneLoader::ProcessRigidbody (TiXmlElement* xmlElem, GameObject* object)
{
	std::string mass = xmlElem->Attribute ("mass");

	object->GetRigidbody ()->SetMass (std::stof (mass));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Collider") {
			ProcessCollider (content, object->GetRigidbody ());
		}

		content = content->NextSiblingElement ();
	}
}

void SceneLoader::ProcessAudioSource (TiXmlElement* xmlElem, GameObject* object)
{
	std::string volume = xmlElem->Attribute ("volume");
	std::string loop = xmlElem->Attribute ("loop");

	object->GetAudioSource ()->SetVolume (std::stof (volume));
	object->GetAudioSource ()->SetLoop (Extensions::StringExtend::ToBool (loop));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "AudioClip") {
			ProcessAudioClip (content, object->GetAudioSource ());
		}

		content = content->NextSiblingElement ();
	}
}

void SceneLoader::ProcessCollider (TiXmlElement* xmlElem, Rigidbody* rigidbody)
{
	std::string path = xmlElem->Attribute ("path");

	BulletCollider* collider = Resources::LoadCollider (path);

	rigidbody->SetCollider (collider);
}

void SceneLoader::ProcessAudioClip (TiXmlElement* xmlElem, AudioSource* audioSource)
{
	std::string path = xmlElem->Attribute ("path");

	Resource<AudioClip> audioClip = Resources::LoadAudioClip (path);

	audioSource->SetAudioClip (audioClip);
}