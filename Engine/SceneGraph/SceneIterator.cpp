#include "SceneIterator.h"

SceneIterator::SceneIterator (Transform* transform)
{
	if (transform == nullptr) {
		return;
	}

	_transforms.push (transform);
	_childIt.push (transform->begin<Transform*> ());
}

SceneIterator::~SceneIterator ()
{

}

SceneIterator& SceneIterator::operator ++ ()
{
	while (_transforms.empty () == false &&
		_childIt.top () == _transforms.top ()->end<Transform*> ()) {

		_childIt.pop ();
		_transforms.pop ();
	}

	if (_transforms.empty () == false) {
		_transforms.push (*_childIt.top ());

		_childIt.top () ++;

		_childIt.push (_transforms.top ()->begin<Transform*> ());
	}

	return *this;
}

bool SceneIterator::operator != (const SceneIterator& other)
{
	return !_transforms.empty ();
}

SceneObject* SceneIterator::operator* ()
{
	return _transforms.top ()->GetSceneObject ();
}
