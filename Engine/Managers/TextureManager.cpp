#include "TextureManager.h"

#include <map>
#include <string>

#include "Resources/Resources.h"

TextureManager::TextureManager ()
{
	std::string defaultTexture = "Assets/Textures/AmbientDefault.png";	

	_default = Resources::LoadTexture (defaultTexture);
}

TextureManager& TextureManager::Instance ()
{
	static TextureManager instance;

	return instance;
}

void TextureManager::AddTexture (Texture* texture)
{
	if (_textures.find (texture->GetName()) != _textures.end ()) {
		return ;
	}

	_textures [texture->GetName()] = texture;
}

Texture* TextureManager::GetTexture (const std::string& filename)
{
	if (_textures.find (filename) == _textures.end ()) {
		return NULL;
	}

	return _textures [filename];
}

Texture* TextureManager::Default()
{
	return _default;
}

