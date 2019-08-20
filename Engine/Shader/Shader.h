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
	GLuint _program;
	std::map<std::string, int> _uniforms;

public:
	Shader (const std::string& name, GLuint program);
	virtual ~Shader ();

	std::string GetName () const;
	GLuint GetProgram () const;

	int GetUniformLocation (const std::string& name);
	unsigned int GetUniformBlockIndex (const std::string& name);
};

#endif