#ifndef SHADERVIEW_H
#define SHADERVIEW_H

#include "Core/Interfaces/Object.h"

#include <map>

class ShaderView : public Object
{
protected:
	unsigned int _program;
	std::map<std::string, int> _uniforms;

public:
	ShaderView (unsigned int program);
	~ShaderView ();

	std::string GetName () const;
	unsigned int GetProgram () const;

	int GetUniformLocation (const std::string& name);
	unsigned int GetUniformBlockIndex (const std::string& name);
	unsigned int GetShaderStorageBlockIndex (const std::string& name);
};

#endif