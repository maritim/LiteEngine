#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include "Core/Singleton/Singleton.h"

class Editor
{
public:
	static void Init ();

	static void Update ();

	static bool WantCaptureInput ();

	static void Clear ();
};

#endif