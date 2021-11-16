#ifndef SCENESAVER_H
#define SCENESAVER_H

#include <tinyxml2.h>

#include "SceneGraph/Scene.h"

class ENGINE_API SceneSaver
{
public:
	static SceneSaver& Instance ();

	void Save (const Scene*, const std::string& filename);
protected:
	SceneSaver ();

	void SaveSkybox (tinyxml2::XMLElement* xmlElem, const Skybox* skybox);
	void SaveSceneObject (tinyxml2::XMLElement* xmlElem, const SceneObject* sceneObject);

	void SaveTransform (tinyxml2::XMLElement* xmlElem, const Transform* transform);
	void SaveComponents (tinyxml2::XMLElement* xmlElem, const SceneObject* sceneObject);

	void SaveComponent (tinyxml2::XMLElement* xmlElem, const Component* component);
};

#endif