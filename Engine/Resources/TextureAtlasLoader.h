#ifndef TEXTUREATLASLOADER_H
#define TEXTUREATLASLOADER_H

#include "ResourceLoader.h"

#include <string>

#include "Texture/TextureAtlas.h"

class TextureAtlasLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);

private:
	void LoadTexture (const std::string& filename, TextureAtlas* texAtlas);
	void LoadAtlas (const std::string& filename, TextureAtlas* texAtlas);

	void ProcessComment (std::ifstream& file);
	void ProcessArea (std::ifstream& file, TextureAtlas* curTexAtlas);
};

#endif