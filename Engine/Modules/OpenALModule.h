#ifndef OPENALMODULE_H
#define OPENALMODULE_H

#include "Wrappers/OpenAL/AL.h"

class OpenALModule
{
private:
	static ALCcontext* _context;

public:
	static void Init ();
	static void Quit ();
};

#endif