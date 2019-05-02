#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "Widgets/EditorWidget.h"

#define REGISTER_EDITOR_WIDGET(EDITOR_WIDGET) static EditorWidgetRegister<EDITOR_WIDGET> dummy (#EDITOR_WIDGET);

class EditorManager : public Singleton<EditorManager>
{
	friend Singleton<EditorManager>;

private:
	std::vector<EditorWidget*> _widgets;

public:
	void Update ();

	void RegisterWidget (EditorWidget*);

	// bool WantCaptureInput ();
private:
	EditorManager ();
	~EditorManager ();
	EditorManager (const EditorManager&);
	EditorManager& operator=(const EditorManager&);
};

template <class EditorWidgetType>
class EditorWidgetRegister : public Object
{
public:
	EditorWidgetRegister (const std::string& name)
	{
		EditorManager::Instance ()->RegisterWidget (new EditorWidgetType ());
	}
};

#endif