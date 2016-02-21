#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <string>

#include "Texture/Texture.h"

class TextureManager
{
private:
	std::map<std::string, Texture*> _textures;
	Texture* _default;
public:
	static TextureManager& Instance ();
	void AddTexture (Texture* texture);
	Texture* GetTexture (const std::string& filename);
	Texture* Default();
private:
	TextureManager ();
};

#endif