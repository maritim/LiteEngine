#include "Shader.h"

Shader::Shader (const std::string& name, unsigned int program) :
	_name(name),
	_program(program),
	_uniforms ()
{

}

Shader::~Shader ()
{

}

std::string Shader::GetName () const
{
	return _name;
}

GLuint Shader::GetProgram () const
{
	return _program;
}

int Shader::GetUniformLocation (const std::string& name)
{
	auto it = _uniforms.find (name);

	if (it != _uniforms.end ()) {
		return it->second;
	}

	int uniformLocation = GL::GetUniformLocation (_program, name.c_str());

	_uniforms [name] = uniformLocation;

	return uniformLocation;
}

unsigned int Shader::GetUniformBlockIndex (const std::string& name)
{
	unsigned int uniformBlockIndex = GL::GetUniformBlockIndex (_program, name.c_str ());

	return uniformBlockIndex;
}
