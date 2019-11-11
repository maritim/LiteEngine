#ifndef SCENEITERATOR_H
#define SCENEITERATOR_H

#include "Core/Interfaces/Object.h"

#include <stack>

#include "SceneObject.h"

class Scene;

class ENGINE_API SceneIterator : public Object
{
	friend class Scene;

private:
	std::stack<Transform*> _transforms;
	std::stack<std::set<Transform*>::iterator> _childIt;

public:
	~SceneIterator ();

	SceneIterator& operator++ ();
	bool operator != (const SceneIterator& other);
	SceneObject* operator* ();
private:
	SceneIterator (Transform*);
};

#endif