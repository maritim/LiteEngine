#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include "Core/Interfaces/Object.h"

class EditorWidget : public Object
{
public:
	virtual ~EditorWidget ();

	virtual void Show () = 0;
};

#endif