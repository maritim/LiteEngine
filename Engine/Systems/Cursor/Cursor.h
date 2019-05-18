#ifndef CURSOR_H
#define CURSOR_H

#include "CursorType.h"

class ENGINE_API Cursor
{
private:
	static SDL_Cursor* _cursors [CURSOR_TYPE_COUNT];

public:
	static void Init ();

	static void Hide ();
	static void Show ();

	static bool IsShown ();

	static void SetCursor (CursorType type);

	static void Clear ();
};

#endif