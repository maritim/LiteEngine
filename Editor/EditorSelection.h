#ifndef EDITOROBJECTPICKER_H
#define EDITOROBJECTPICKER_H

#include "Core/Singleton/Singleton.h"

#include "SceneGraph/SceneObject.h"

class EditorSelection : public Singleton<EditorSelection>
{
	friend Singleton<EditorSelection>;

private:
	std::vector<SceneObject*> _focusedObjects;

public:
	void SetActive (SceneObject* object);
	SceneObject* GetActive ();
private:
	EditorSelection ();
	~EditorSelection ();
	EditorSelection (const EditorSelection&);
	EditorSelection& operator=(const EditorSelection&);
};

#endif