#ifndef SCENESAVER_H
#define SCENESAVER_H

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "SceneGraph/Scene.h"

class SceneSaver
{
public:
	static SceneSaver& Instance ();

	void Save (const Scene*, const std::string& filename);
protected:
	SceneSaver ();

	void SaveSkybox (TiXmlElement* xmlElem, const Skybox* skybox);
	void SaveSceneObject (TiXmlElement* xmlElem, const SceneObject* sceneObject);

	void SaveTransform (TiXmlElement* xmlElem, const Transform* transform);
	void SaveComponents (TiXmlElement* xmlElem, const SceneObject* sceneObject);

	void SaveComponent (TiXmlElement* xmlElem, const Component* component);
};

#endif