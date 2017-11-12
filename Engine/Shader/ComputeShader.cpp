#include "ComputeShader.h"

ComputeShader::ComputeShader (const std::string& name, GLuint program,
	GLuint compute) :
	Shader (name, program),
	_computeShader (compute)
{

}

ComputeShader::~ComputeShader ()
{
	GL::DetachShader (_program, _computeShader);

	GL::DeleteShader (_computeShader);

	GL::DeleteProgram (_program);
}

GLuint ComputeShader::GetComputeShader () const
{
	return _computeShader;
}

void ComputeShader::SetComputeFilename (const std::string& name)
{
	_computeFilename = name;
}


