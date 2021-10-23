#include "ShaderView.h"

#include "Wrappers/OpenGL/GL.h"

ShaderView::ShaderView (unsigned int program) :
	_program(program),
	_uniforms ()
{

}

ShaderView::~ShaderView ()
{
	/*
	 * Delete shader
	*/

	GL::DeleteProgram (_program);
}

unsigned int ShaderView::GetProgram () const
{
	return _program;
}

int ShaderView::GetUniformLocation (const std::string& name)
{
	auto it = _uniforms.find (name);

	if (it != _uniforms.end ()) {
		return it->second;
	}

	int uniformLocation = GL::GetUniformLocation (_program, name.c_str());

	_uniforms [name] = uniformLocation;

	return uniformLocation;
}

unsigned int ShaderView::GetUniformBlockIndex (const std::string& name)
{
	unsigned int uniformBlockIndex = GL::GetUniformBlockIndex (_program, name.c_str ());

	return uniformBlockIndex;
}

unsigned int ShaderView::GetShaderStorageBlockIndex (const std::string& name)
{
	unsigned int ssboBlockIndex = GL::GetProgramResourceIndex (_program, GL_SHADER_STORAGE_BLOCK, name.c_str ());

	return ssboBlockIndex;
}
