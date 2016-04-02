#ifndef SHADER_H
#define SHADER_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <map>

class Shader : public Object
{
protected:
	std::string _name;
	std::map<std::string, int> _uniforms;
	std::string _vertexFilename;
	std::string _fragmentFilename;
	unsigned int _program;
	unsigned int _vertexShader;
	unsigned int _fragmentShader;

public:
	Shader (const std::string& name, unsigned int program, 
		unsigned int vertex, unsigned int fragment);
	~Shader ();

	unsigned int GetProgram () const;
	unsigned int GetVertexShader () const;
	unsigned int GetFragmentShader () const;

	void SetVertexFilename (const std::string& name);
	void SetFragmentFilename (const std::string& name);

	int GetUniformLocation (const std::string& name);
};

#endif