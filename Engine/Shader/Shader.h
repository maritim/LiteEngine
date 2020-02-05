#ifndef SHADERI_H
#define SHADERI_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <map>

#include "Wrappers/OpenGL/GL.h"

class Shader : public Object
{
protected:
	std::string _name;

public:
	Shader (const std::string& name);

	std::string GetName () const;
};

#endif