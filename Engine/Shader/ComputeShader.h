#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Shader.h"

#include "Core/Resources/Resource.h"
#include "ShaderContent.h"

class ComputeShader : public Shader
{
protected:
	Resource<ShaderContent> _computeShaderContent;

public:
	using Shader::Shader;

	const Resource<ShaderContent>& GetComputeShaderContent () const;

	void SetComputeShaderContent (const Resource<ShaderContent>& shaderContent);
};

#endif