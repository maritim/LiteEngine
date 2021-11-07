#ifndef EDITOR_H
#define EDITOR_H

#include "Main/GameModule.h"

class Editor : public GameModule
{
public:
	void Init ();
	void UpdateFrame ();
	void UpdateScene ();
	void RenderScene ();
protected:
	void LoadEditorLayout ();
};

REGISTER_GAME_MODULE(Editor)

#endif