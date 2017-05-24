#ifndef SCENEITERATOR_H
#define SCENEITERATOR_H

#include "Core/Interfaces/Object.h"

#include "Scene.h"
#include "SceneObject.h"

class Scene;

class SceneIterator : public Object
{
	friend class Scene;

private:
	std::size_t _currentPos;
	Scene* _scene;

public:
	~SceneIterator ();

	SceneIterator& operator++ ();
	bool operator != (const SceneIterator& other);
	SceneObject* operator* ();
private:
	SceneIterator (Scene* scene, std::size_t startPos);
};

#endif