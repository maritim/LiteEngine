#include "DrawingShader.h"

DrawingShader::DrawingShader (const std::string& name, GLuint program,
	unsigned int vertex, unsigned int fragment, unsigned int geometry) :
	Shader (name, program),
	_vertexShader (vertex),
	_geometryShader (geometry),
	_fragmentShader (fragment)
{

}

DrawingShader::~DrawingShader ()
{
	GL::DetachShader (_program, _vertexShader);
	GL::DetachShader (_program, _fragmentShader);

	GL::DeleteShader (_vertexShader);
	GL::DeleteShader (_fragmentShader);

	GL::DeleteProgram (_program);
}

GLuint DrawingShader::GetVertexShader () const
{
	return _vertexShader;
}

GLuint DrawingShader::GetGeometryShader () const
{
	return _geometryShader;
}

GLuint DrawingShader::GetFragmentShader () const
{
	return _fragmentShader;
}

void DrawingShader::SetVertexFilename (const std::string& name)
{
	_vertexFilename = name;
}

void DrawingShader::SetGeometryFilename (const std::string& name)
{
	_geometryFilename = name;
}

void DrawingShader::SetFragmentFilename (const std::string& name)
{
	_fragmentFilename = name;
}