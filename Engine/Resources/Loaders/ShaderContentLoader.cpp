#include "ShaderContentLoader.h"

#include <fstream>

#include "Shader/ShaderContent.h"

#include "Resources/Resources.h"

#include "Core/Console/Console.h"

Object* ShaderContentLoader::Load (const std::string& filename)
{
	ShaderContent* shaderContent = new ShaderContent ();

	std::ifstream objFile (filename.c_str());

	if (!objFile.is_open ()) {
		Console::LogError ("Unable to load \"" + filename + "\" shader!");

		exit (0);
	}

	shaderContent->SetFilename (filename);

	std::string content;
	std::string line;

	while (std::getline (objFile, line)) {

		//TODO: Check comments
		if (line.find ("#include") != std::string::npos) {
			ProcessInclude (line, content, filename);
		} else {
			content += line + '\n';
		}
	}

	shaderContent->SetContent (content);

	return shaderContent;
}

void ShaderContentLoader::ProcessInclude (const std::string& line, std::string& content, const std::string& filename)
{
	std::string shadersDirectory = "Assets/Shaders/";

	std::string includeFilename = line.substr (line.find ('\"') + 1, line.rfind ('\"') - line.find ('\"') - 1);
	std::string includeFilePath = shadersDirectory + includeFilename;

	Resource<ShaderContent> shaderContent = Resources::LoadShaderContent (includeFilePath);

	content += shaderContent->GetContent ();
}
