#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <glm/vec3.hpp>
#include <string>
#include <tinyxml2.h>

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneObject.h"

class SceneLoader
{
public:
	static SceneLoader& Instance ();

	Scene* Load (const std::string& filename);
private:
	SceneLoader ();

	void ProcessSkybox (tinyxml2::XMLElement* xmlElem, Scene* scene);
	void ProcessSceneObject (tinyxml2::XMLElement* xmlElem, Scene* scene);
	void ProcessParticleSystem (tinyxml2::XMLElement* xmlElem, Scene* scene);

	void ProcessTransform (tinyxml2::XMLElement* xmlElem, Scene* scene, SceneObject* obj);
	glm::vec3 GetPosition (tinyxml2::XMLElement* xmlElem);
	glm::quat GetRotation (tinyxml2::XMLElement* xmlElem);
	glm::vec3 GetScale (tinyxml2::XMLElement* xmlElem);

	void ProcessComponents (tinyxml2::XMLElement* xmlElem, SceneObject* sceneObject);
	void ProcessComponent (tinyxml2::XMLElement* xmlElem, SceneObject* sceneObject);
};

#endif