#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include "SceneObject.h"
#include "Skybox/Skybox.h"

class Scene
{
private:
	std::vector<SceneObject*> _sceneObjects;
protected:
	std::string _name;
	Skybox* _skybox;
public:
	Scene ();
	virtual ~Scene ();

	static Scene* Current ();

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

	void Update ();
};

#endif