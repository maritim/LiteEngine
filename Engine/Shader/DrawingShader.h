#ifndef SHADER_H
#define SHADER_H

#include "Shader.h"

#include "Core/Resources/Resource.h"
#include "ShaderContent.h"

class DrawingShader : public Shader
{
protected:
	Resource<ShaderContent> _vertexShaderContent;
	Resource<ShaderContent> _geometryShaderContent;
	Resource<ShaderContent> _fragmentShaderContent;

public:
	using Shader::Shader;

	const Resource<ShaderContent>& GetVertexShaderContent () const;
	const Resource<ShaderContent>& GetGeometryShaderContent () const;
	const Resource<ShaderContent>& GetFragmentShaderContent () const;

	void SetVertexShaderContent (const Resource<ShaderContent>& shaderContent);
	void SetGeometryShaderContent (const Resource<ShaderContent>& shaderContent);
	void SetFragmentShaderContent (const Resource<ShaderContent>& shaderContent);
};

#endif