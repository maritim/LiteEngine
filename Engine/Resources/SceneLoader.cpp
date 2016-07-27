#include "SceneLoader.h"

#include <string>

#include "SceneNodes/GameObject.h"
#include "SceneNodes/AnimationGameObject.h"
#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "Mesh/Model.h"
#include "Skybox/Skybox.h"

#include "Systems/Components/ComponentsFactory.h"
#include "Systems/Components/Component.h"

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

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Light") {
			ProcessLight (content, scene);
		}
		else if (name == "Skybox") {
			ProcessSkybox (content, scene);
		}
		else if (name == "GameObject") {
			ProcessGameObject (content, scene);
		}
		else if (name == "AnimatedGameObject") {
			ProcessAnimationGameObject (content, scene);
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

void SceneLoader::ProcessGameObject (TiXmlElement* xmlElem, Scene* scene)
{
	std::string name = xmlElem->Attribute ("name");
	std::string instanceID = xmlElem->Attribute ("InstanceID");
	std::string meshPath = xmlElem->Attribute ("meshpath");

	GameObject* gameObject = new GameObject ();
	gameObject->SetName (name);
	// Need unsigned int here
	gameObject->SetInstanceID (std::stoi (instanceID));

	Model* mesh = Resources::LoadModel (meshPath);
	gameObject->AttachMesh (mesh);

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
		else if (name == "Components") {
			ProcessComponents (content, gameObject);
		}

		content = content->NextSiblingElement ();
	}

	scene->AttachObject (gameObject);
}

void SceneLoader::ProcessAnimationGameObject (TiXmlElement* xmlElem, Scene* scene)
{
	std::string name = xmlElem->Attribute ("name");
	std::string instanceID = xmlElem->Attribute ("InstanceID");
	std::string meshPath = xmlElem->Attribute ("meshpath");

	AnimationGameObject* animGameObject = new AnimationGameObject ();
	animGameObject->SetName (name);
	// Need unsigned int here
	animGameObject->SetInstanceID (std::stoi (instanceID));

	Model* mesh = Resources::LoadAnimatedModel (meshPath);
	animGameObject->AttachMesh (mesh);

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Transform") {
			ProcessTransform (content, scene, animGameObject);
		}
		else if (name == "Rigidbody") {
			ProcessRigidbody (content, animGameObject);
		}
		else if (name == "Components") {
			ProcessComponents (content, animGameObject);
		}

		content = content->NextSiblingElement ();
	}

	scene->AttachObject (animGameObject);
}

void SceneLoader::ProcessParticleSystem (TiXmlElement* xmlElem, Scene* scene)
{
	std::string name = xmlElem->Attribute ("name");
	std::string instanceID = xmlElem->Attribute ("InstanceID");
	std::string path = xmlElem->Attribute ("path");

	ParticleSystem* partSystem = Resources::LoadParticleSystem (path);
	partSystem->SetName (name);
	// Need unsigned int here
	partSystem->SetInstanceID (std::stoi (instanceID));

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Transform") {
			ProcessTransform (content, scene, partSystem);
		}
		else if (name == "Rigidbody") {
			ProcessRigidbody (content, partSystem);
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

	Light* light = Resources::LoadLight (path);
	light->SetName (name);
	light->SetInstanceID (std::stoi (instanceID));

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
	Transform transform;

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Position") {
			transform.SetPosition (GetPosition (content));
		}
		else if (name == "Rotation") {
			transform.SetRotation (GetRotation (content));
		}
		else if (name == "Scale") {
			transform.SetScale (GetScale (content));
		}

		content = content->NextSiblingElement ();
	}

	const char* parentName = xmlElem->Attribute ("parent");
	if (parentName != NULL) {
		SceneObject* parent =  scene->GetObject (parentName);

		sceneObject->GetTransform ()->SetParent (parent->GetTransform ());
	}	

	sceneObject->GetTransform ()->SetPosition (transform.GetPosition ());
	sceneObject->GetTransform ()->SetRotation (transform.GetRotation ());
	sceneObject->GetTransform ()->SetScale (transform.GetScale ());
}

glm::vec3 SceneLoader::GetPosition (TiXmlElement* xmlElem)
{
	glm::vec3 vector3;

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
	const char* name = xmlElem->Attribute ("name");

	if (name == NULL) {
		return;
	}

	Component* component = ComponentsFactory::Instance ()->Create ((std::string) name);

	gameObject->AttachComponent (component);	
}

void SceneLoader::ProcessRigidbody (TiXmlElement* xmlElem, SceneObject* object)
{
	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Gravity") {
			ProcessGravity (content, object->GetRigidbody ());
		}

		content = content->NextSiblingElement ();
	}
}

void SceneLoader::ProcessGravity (TiXmlElement* xmlElem, Rigidbody* rigidbody)
{
	bool useGravity = Extensions::StringExtend::ToBool (xmlElem->Attribute ("use"));

	rigidbody->SetGravityUse (useGravity);
}