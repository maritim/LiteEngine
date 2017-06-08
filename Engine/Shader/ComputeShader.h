#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Shader.h"

class ComputeShader : public Shader
{
protected:
	std::string _computeFilename;
	GLuint _computeShader;

public:
	ComputeShader (const std::string& name, GLuint program, 
		GLuint compute);
	~ComputeShader ();

	GLuint GetComputeShader () const;

	void SetComputeFilename (const std::string& name);
};

#endif