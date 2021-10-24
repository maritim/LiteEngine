#include "TextureAtlasLoader.h"

#include <fstream>
#include <limits>
#include <string>

#include "Resources/Resources.h"

#include "Renderer/Render/Texture/Texture.h"

#include "Core/Console/Console.h"

#include "Utils/Files/FileSystem.h"
#include "Utils/Extensions/StringExtend.h"

Object* TextureAtlasLoader::Load (const std::string& filename)
{
	TextureAtlas* texAtlas = new TextureAtlas ("");

	LoadTexture (filename, texAtlas);

	std::string atlFilename = filename;
	std::string extension = FileSystem::GetExtension (filename);
	Extensions::StringExtend::Replace (atlFilename, extension, "");
	atlFilename += ".atlas";

	LoadAtlas (atlFilename, texAtlas);

	return texAtlas;
}

void TextureAtlasLoader::LoadTexture (const std::string& filename, TextureAtlas* texAtlas)
{
	Resource<Texture> texture = Resources::LoadTexture (filename);

	texAtlas->SetName (texture->GetName ());
	texAtlas->SetSize (texture->GetSize ());
	texAtlas->SetPixels (texture->GetPixels (), 4u * texture->GetSize ().width * texture->GetSize ().height);
}

void TextureAtlasLoader::LoadAtlas (const std::string& filename, TextureAtlas* texAtlas)
{
	std::string lineType;

	std::ifstream file (filename.c_str ());

	if (!file.is_open ()) {
		Console::LogError ("Unable to open atlas file \"" + filename + "\" !");

		exit (1);
	}

	while (file >> lineType) {
		if (lineType == "#") {
			ProcessComment (file);
		}
		else if (lineType == "Area") {
			ProcessArea (file, texAtlas);
		} else {
			ProcessComment (file);
		}
	}

	file.close ();
}

void TextureAtlasLoader::ProcessComment (std::ifstream& file)
{
	file.ignore (std::numeric_limits<std::streamsize>::max(), '\n');	
}

void TextureAtlasLoader::ProcessArea (std::ifstream& file, TextureAtlas* atlas)
{
	float startX, startY, width, height;

	file >> startX >> startY >> width >> height;

	atlas->AddTextureArea (startX, startY, width, height);
}