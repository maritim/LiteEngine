#include "SceneSaver.h"

#include "Systems/Components/PersistentComponent.h"

#include "Utils/Extensions/MathExtend.h"

using namespace tinyxml2;

SceneSaver::SceneSaver ()
{

}

SceneSaver& SceneSaver::Instance ()
{
	static SceneSaver sceneSaver;

	return sceneSaver;
}

void SceneSaver::Save (const Scene* scene, const std::string& filename)
{
	XMLDocument doc;

	XMLDeclaration* declaration = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	XMLElement* xmlRoot = doc.NewElement("Scene");

	xmlRoot->SetAttribute ("name", scene->GetName ().c_str ());

	SaveSkybox (xmlRoot, scene->GetSkybox ());

	const SceneRoot* sceneRoot = scene->GetRoot ();

	for_each_type (Transform*, child, *sceneRoot->GetTransform ()) {
		SaveSceneObject (xmlRoot, child->GetSceneObject ());
	}

    doc.LinkEndChild(declaration);
    doc.LinkEndChild(xmlRoot);

	doc.SaveFile (filename.c_str ());
}

void SceneSaver::SaveSkybox (XMLElement* xmlRoot, const Skybox* skybox)
{
	if (skybox == nullptr) {
		return;
	}

	XMLElement* xmlElem = xmlRoot->InsertNewChildElement("Skybox");
	xmlElem->SetAttribute ("path", skybox->GetName ().c_str ());
}

void SceneSaver::SaveSceneObject (XMLElement* xmlRoot, const SceneObject* sceneObject)
{
	XMLElement* xmlElem = xmlRoot->InsertNewChildElement("SceneObject");

	xmlElem->SetAttribute ("instanceID", std::to_string (sceneObject->GetInstanceID ()).c_str ());
	xmlElem->SetAttribute ("name", sceneObject->GetName ().c_str ());
	xmlElem->SetAttribute ("isActive", sceneObject->IsActive () ? "true" : "false");

    SaveTransform (xmlElem, sceneObject->GetTransform ());
    SaveComponents (xmlElem, sceneObject);

	for_each_type (Transform*, child, *sceneObject->GetTransform ()) {
		SaveSceneObject (xmlRoot, child->GetSceneObject ());
	}
}

void SceneSaver::SaveTransform (XMLElement* xmlElem, const Transform* transform)
{
	XMLElement* transformElem = xmlElem->InsertNewChildElement("Transform");

	if (transform->GetParent ()->GetParent () != nullptr) {
		transformElem->SetAttribute ("parentID", std::to_string (transform->GetParent ()->GetSceneObject ()->GetInstanceID ()).c_str ());
	}

	XMLElement* positionElem = transformElem->InsertNewChildElement("Position");

	const glm::vec3& position = transform->GetPosition ();
	positionElem->SetAttribute ("x", std::to_string (position.x).c_str ());
	positionElem->SetAttribute ("y", std::to_string (position.y).c_str ());
	positionElem->SetAttribute ("z", std::to_string (position.z).c_str ());

	XMLElement* rotationElem = transformElem->InsertNewChildElement("Rotation");

	glm::quat objectRot = transform->GetRotation ();
	glm::vec3 rotation = glm::eulerAngles (objectRot) * RAD2DEG;
	rotationElem->SetAttribute ("x", std::to_string (rotation.x).c_str ());
	rotationElem->SetAttribute ("y", std::to_string (rotation.y).c_str ());
	rotationElem->SetAttribute ("z", std::to_string (rotation.z).c_str ());

	XMLElement* scaleElem = transformElem->InsertNewChildElement("Scale");

	const glm::vec3& scale = transform->GetScale ();
	scaleElem->SetAttribute ("x", std::to_string (scale.x).c_str ());
	scaleElem->SetAttribute ("y", std::to_string (scale.y).c_str ());
	scaleElem->SetAttribute ("z", std::to_string (scale.z).c_str ());
}

void SceneSaver::SaveComponents (XMLElement* xmlElem, const SceneObject* sceneObject)
{
	XMLElement* componentsElem = xmlElem->InsertNewChildElement("Components");

	for_each_type (Component*, component, *sceneObject) {
		SaveComponent (componentsElem, component);
	}
}

void SceneSaver::SaveComponent (XMLElement* xmlElem, const Component* component)
{
	XMLElement* componentElem = xmlElem->InsertNewChildElement("Component");
	componentElem->SetAttribute ("name", component->GetName ().c_str ());

	auto persistentComponent = dynamic_cast<const PersistentComponent*> (component);

	if (persistentComponent == nullptr) {
		return;
	}

	persistentComponent->Save (componentElem);
}
