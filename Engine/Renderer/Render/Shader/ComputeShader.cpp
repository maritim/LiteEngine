#include "ComputeShader.h"

const Resource<ShaderContent>& ComputeShader::GetComputeShaderContent () const
{
	return _computeShaderContent;
}

void ComputeShader::SetComputeShaderContent (const Resource<ShaderContent>& shaderContent)
{
	_computeShaderContent = shaderContent;
}
