#ifndef EDITORANIMATIONMODEL_H
#define EDITORANIMATIONMODEL_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorAnimationModel : public EditorWidget
{
protected:
	int _currentAnimIndex;
	bool _isBlending;
	float _blendDuration;

public:
	EditorAnimationModel ();

	void Show ();
protected:
	void ShowAnimationsWindow ();
};

REGISTER_EDITOR_WIDGET(EditorAnimationModel)

#endif