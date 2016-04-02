#include "Shader.h"

#include "Wrappers/OpenGL/GL.h"

Shader::Shader (const std::string& name, unsigned int program, 
	unsigned int vertex, unsigned int fragment) :
	_name (name),
	_program (program),
	_vertexShader (vertex),
	_fragmentShader (fragment)
{

}

Shader::~Shader ()
{

}

unsigned int Shader::GetProgram () const
{
	return _program;
}

unsigned int Shader::GetVertexShader () const
{
	return _vertexShader;
}

unsigned int Shader::GetFragmentShader () const
{
	return _fragmentShader;
}

void Shader::SetVertexFilename (const std::string& name)
{
	_vertexFilename = name;
}

void Shader::SetFragmentFilename (const std::string& name)
{
	_fragmentFilename = name;
}

int Shader::GetUniformLocation (const std::string& name)
{
	auto it = _uniforms.find (name);

	if (it != _uniforms.end ()) {
		return it->second;
	}

	int uniformLocation = GL::GetUniformLocation (_program, name.c_str ());

	_uniforms [name] = uniformLocation;

	return uniformLocation;
}