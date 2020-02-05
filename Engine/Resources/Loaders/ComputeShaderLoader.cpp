#include "ComputeShaderLoader.h"

#include "Shader/ComputeShader.h"

#include "Resources/Resources.h"

Object* ComputeShaderLoader::Load(const std::string& filename)
{
	ComputeShader* shader = new ComputeShader (filename);

	Resource<ShaderContent> shaderContent = Resources::LoadShaderContent (filename);

	shader->SetComputeShaderContent (shaderContent);

	return shader;
}
