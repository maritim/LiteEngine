#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "Texture/Texture.h"

class TextureManager : public Singleton<TextureManager>
{
	friend Singleton<TextureManager>;

private:
	std::map<std::string, Texture*> _textures;

public:
	void AddTexture (Texture* texture);
	Texture* GetTexture (const std::string& filename);
private:
	TextureManager ();
	~TextureManager ();
	TextureManager (const TextureManager&);
	TextureManager& operator=(const TextureManager&);

	void LoadInGPU (Texture*);
};

#endif