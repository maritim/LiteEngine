#ifndef SCENE_H
#define SCENE_H

#include "Core/Interfaces/Object.h"

#include <string>

#include "SceneObject.h"
#include "Skybox/Skybox.h"

#include "SceneRoot.h"

#include "Core/Intersections/AABBVolume.h"

#include "SceneIterator.h"

class SceneIterator;

class ENGINE_API Scene : public Object
{
	friend class SceneIterator;

protected:
	SceneRoot* _sceneRoot;

	std::string _name;
	Skybox* _skybox;
	AABBVolume _boundingBox;

public:
	Scene ();
	virtual ~Scene ();

	SceneRoot* GetRoot () const;

	void SetName (const std::string& name);
	std::string GetName () const;

	void SetSkybox (Skybox* skybox);
	Skybox* GetSkybox () const;

	void AttachObject (SceneObject*);
	void DetachObject (SceneObject*);

	SceneObject* GetObject (const std::string& name) const;

	const AABBVolume& GetBoundingBox () const;

	SceneIterator begin () const;
	SceneIterator end () const;

	void Update ();
protected:
	void UpdateBoundingBox (SceneObject* object);

	void Clear (SceneObject* object);
};

#endif