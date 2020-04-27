#ifndef SCENE_H
#define SCENE_H

#include "Core/Interfaces/Object.h"

#include <string>

#include "SceneObject.h"
#include "Skybox/Skybox.h"

#include "SceneRoot.h"

#include "Core/Intersections/AABBVolume.h"

#include "SceneIterator.h"

class ENGINE_API Scene : public Object
{
protected:
	SceneRoot* _sceneRoot;

	std::string _path;
	std::string _name;
	Skybox* _skybox;
	AABBVolume _boundingBox;

	std::vector<SceneObject*> _needRemoveObjects;

public:
	Scene ();
	virtual ~Scene ();

	SceneRoot* GetRoot () const;

	void SetPath (const std::string& path);
	const std::string& GetPath () const;

	void SetName (const std::string& name);
	const std::string& GetName () const;

	void SetSkybox (Skybox* skybox);
	Skybox* GetSkybox () const;

	void AttachObject (SceneObject*);
	void DetachObject (SceneObject*);

	SceneObject* GetObject (const std::string& name) const;
	SceneObject* GetObject (std::size_t instanceID) const;

	const AABBVolume& GetBoundingBox () const;

	SceneIterator begin () const;
	SceneIterator end () const;

	void Update ();
protected:
	void UpdateBoundingBox (SceneObject* object);

	void Clear (SceneObject* object);
};

#endif