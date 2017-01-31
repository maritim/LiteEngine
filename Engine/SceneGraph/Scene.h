#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include "SceneObject.h"
#include "Skybox/Skybox.h"

#include "Core/Intersections/AABBVolume.h"

class Scene
{
private:
	std::vector<SceneObject*> _sceneObjects;
protected:
	std::string _name;
	Skybox* _skybox;
	AABBVolume* _boundingBox;

public:
	Scene ();
	virtual ~Scene ();

	void Init ();

	void SetName (const std::string& name);
	std::string GetName () const;

	void SetSkybox (Skybox* sky);
	Skybox* GetSkybox () const;

	void AttachObject (SceneObject*);
	void DetachObject (SceneObject*);

	SceneObject* GetObjectAt (std::size_t index) const;
	SceneObject* GetObject (const std::string& name) const;

	std::size_t GetObjectsCount () const;	
	AABBVolume* GetBoundingBox () const;

	void Update ();
private:
	void UpdateBoundingBox (SceneObject* object);
};

#endif