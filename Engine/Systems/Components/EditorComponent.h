#ifndef EDITORCOMPONENT_H
#define EDITORCOMPONENT_H

#include "PersistentComponent.h"

class EditorComponent : public PersistentComponent
{
public:
	virtual void Show () = 0;
};

#endif