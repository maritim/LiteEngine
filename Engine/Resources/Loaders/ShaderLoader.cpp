#include "ShaderLoader.h"

#include "Shader/DrawingShader.h"

#include "Resources/Resources.h"

Object* ShaderLoader::Load(const std::string& filename)
{
	DrawingShader* shader = new DrawingShader (filename);

	Resource<ShaderContent> vertexShaderContent = Resources::LoadShaderContent (_filenames [0]);
	Resource<ShaderContent> fragmentShaderContent = Resources::LoadShaderContent (_filenames [1]);

	shader->SetVertexShaderContent (vertexShaderContent);
	shader->SetFragmentShaderContent (fragmentShaderContent);

	if (_filenames.size () > 2) {
		Resource<ShaderContent> geometryShaderContent = Resources::LoadShaderContent (_filenames [2]);

		shader->SetGeometryShaderContent (geometryShaderContent);
	}

	return shader;
}

void ShaderLoader::SetFilenames(const std::vector<std::string>& filenames)
{
	_filenames = filenames;
}