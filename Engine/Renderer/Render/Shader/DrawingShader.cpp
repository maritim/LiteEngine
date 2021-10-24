#include "DrawingShader.h"

const Resource<ShaderContent>& DrawingShader::GetVertexShaderContent () const
{
	return _vertexShaderContent;
}

const Resource<ShaderContent>& DrawingShader::GetGeometryShaderContent () const
{
	return _geometryShaderContent;
}

const Resource<ShaderContent>& DrawingShader::GetFragmentShaderContent () const
{
	return _fragmentShaderContent;
}

void DrawingShader::SetVertexShaderContent (const Resource<ShaderContent>& shaderContent)
{
	_vertexShaderContent = shaderContent;
}

void DrawingShader::SetGeometryShaderContent (const Resource<ShaderContent>& shaderContent)
{
	_geometryShaderContent = shaderContent;
}

void DrawingShader::SetFragmentShaderContent (const Resource<ShaderContent>& shaderContent)
{
	_fragmentShaderContent = shaderContent;
}