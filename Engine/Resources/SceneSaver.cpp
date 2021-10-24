#include "SceneSaver.h"

#include "Systems/Components/PersistentComponent.h"

#include "Utils/Extensions/MathExtend.h"

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
	TiXmlDocument doc;

    TiXmlDeclaration * declaration = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	TiXmlElement* xmlRoot = new TiXmlElement ("Scene");

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

void SceneSaver::SaveSkybox (TiXmlElement* xmlRoot, const Skybox* skybox)
{
	if (skybox == nullptr) {
		return;
	}

	TiXmlElement* xmlElem = new TiXmlElement ("Skybox");

	xmlElem->SetAttribute ("path", skybox->GetName ().c_str ());

    xmlRoot->LinkEndChild(xmlElem);
}

void SceneSaver::SaveSceneObject (TiXmlElement* xmlRoot, const SceneObject* sceneObject)
{
	TiXmlElement* xmlElem = new TiXmlElement ("SceneObject");

	xmlElem->SetAttribute ("instanceID", std::to_string (sceneObject->GetInstanceID ()).c_str ());
	xmlElem->SetAttribute ("name", sceneObject->GetName ().c_str ());
	xmlElem->SetAttribute ("isActive", sceneObject->IsActive () ? "true" : "false");

    xmlRoot->LinkEndChild(xmlElem);

    SaveTransform (xmlElem, sceneObject->GetTransform ());
    SaveComponents (xmlElem, sceneObject);

	for_each_type (Transform*, child, *sceneObject->GetTransform ()) {
		SaveSceneObject (xmlRoot, child->GetSceneObject ());
	}
}

void SceneSaver::SaveTransform (TiXmlElement* xmlElem, const Transform* transform)
{
	TiXmlElement* transformElem = new TiXmlElement ("Transform");

	if (transform->GetParent ()->GetParent () != nullptr) {
		transformElem->SetAttribute ("parentID", std::to_string (transform->GetParent ()->GetSceneObject ()->GetInstanceID ()).c_str ());
	}

	TiXmlElement* positionElem = new TiXmlElement ("Position");

	const glm::vec3& position = transform->GetPosition ();
	positionElem->SetAttribute ("x", std::to_string (position.x).c_str ());
	positionElem->SetAttribute ("y", std::to_string (position.y).c_str ());
	positionElem->SetAttribute ("z", std::to_string (position.z).c_str ());

    transformElem->LinkEndChild(positionElem);

	TiXmlElement* rotationElem = new TiXmlElement ("Rotation");

	glm::quat objectRot = transform->GetRotation ();
	glm::vec3 rotation = glm::eulerAngles (objectRot) * RAD2DEG;
	rotationElem->SetAttribute ("x", std::to_string (rotation.x).c_str ());
	rotationElem->SetAttribute ("y", std::to_string (rotation.y).c_str ());
	rotationElem->SetAttribute ("z", std::to_string (rotation.z).c_str ());

    transformElem->LinkEndChild(rotationElem);

	TiXmlElement* scaleElem = new TiXmlElement ("Scale");

	const glm::vec3& scale = transform->GetScale ();
	scaleElem->SetAttribute ("x", std::to_string (scale.x).c_str ());
	scaleElem->SetAttribute ("y", std::to_string (scale.y).c_str ());
	scaleElem->SetAttribute ("z", std::to_string (scale.z).c_str ());

    transformElem->LinkEndChild(scaleElem);

    xmlElem->LinkEndChild(transformElem);
}

void SceneSaver::SaveComponents (TiXmlElement* xmlElem, const SceneObject* sceneObject)
{
	TiXmlElement* componentsElem = new TiXmlElement ("Components");

	for_each_type (Component*, component, *sceneObject) {
		SaveComponent (componentsElem, component);
	}

    xmlElem->LinkEndChild(componentsElem);
}

void SceneSaver::SaveComponent (TiXmlElement* xmlElem, const Component* component)
{
	TiXmlElement* componentElem = new TiXmlElement ("Component");

	componentElem->SetAttribute ("name", component->GetName ().c_str ());

    xmlElem->LinkEndChild(componentElem);

	auto persistentComponent = dynamic_cast<const PersistentComponent*> (component);

	if (persistentComponent == nullptr) {
		return;
	}

	persistentComponent->Save (componentElem);
}
