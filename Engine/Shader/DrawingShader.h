#ifndef SHADER_H
#define SHADER_H

#include "Shader.h"

class DrawingShader : public Shader
{
protected:
	std::string _vertexFilename;
	std::string _geometryFilename;
	std::string _fragmentFilename;
	GLuint _vertexShader;
	GLuint _geometryShader;
	GLuint _fragmentShader;

public:
	DrawingShader (const std::string& name, GLuint program,
		GLuint vertex, GLuint fragment, GLuint geometry = 0);
	~DrawingShader ();

	GLuint GetVertexShader () const;
	GLuint GetGeometryShader () const;
	GLuint GetFragmentShader () const;

	void SetVertexFilename (const std::string& name);
	void SetGeometryFilename (const std::string& name);
	void SetFragmentFilename (const std::string& name);
};

#endif