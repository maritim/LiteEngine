#include "EditorSelection.h"

EditorSelection::EditorSelection ()
{

}

EditorSelection::~EditorSelection ()
{

}

void EditorSelection::SetActive (SceneObject* object)
{
	_focusedObjects.clear ();

	_focusedObjects.push_back (object);
}

SceneObject* EditorSelection::GetActive ()
{
	if (_focusedObjects.size () == 0) {
		return nullptr;
	}

	return _focusedObjects [0];
}