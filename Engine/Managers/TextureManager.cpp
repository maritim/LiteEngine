#include "TextureManager.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <map>
#include <string>

#include "Resources/Resources.h"
#include "Wrappers/OpenGL/GL.h"

TextureManager::TextureManager ()
{
	std::string defaultTexture = "Assets/Textures/AmbientDefault.png";	
	
	_default = Resources::LoadTexture (defaultTexture);
	this->AddTexture (_default);
}

TextureManager::~TextureManager ()
{

}

void TextureManager::AddTexture (Texture* texture)
{
	if (_textures.find (texture->GetName()) != _textures.end ()) {
		return ;
	}

	/*
	 * TODO: Remove this after TextureManager complete refactorization
	*/

	if (texture->GetPixels () != nullptr) {
		this->LoadInGPU (texture);
	}

	_textures [texture->GetName()] = texture;
}

Texture* TextureManager::GetTexture (const std::string& filename)
{
	if (_textures.find (filename) == _textures.end ()) {
		return nullptr;
	}

	return _textures [filename];
}

Texture* TextureManager::Default()
{
	return _default;
}

/*
 * TODO: Extend this to accept external Texture settings
*/

void TextureManager::LoadInGPU (Texture* texture)
{
	unsigned int gpuIndex = 0;

	Size size (texture->GetSize ());

	GL::GenTextures(1, &gpuIndex);
	GL::BindTexture(GL_TEXTURE_2D, gpuIndex);

	GL::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizs.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetPixels ());

	/*
	 * Wrap Mode
	*/

	if (texture->GetWrapMode () == TEXTURE_WRAP_MODE::WRAP_REPEAT) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);		
	}
	else if (texture->GetWrapMode () == TEXTURE_WRAP_MODE::WRAP_CLAMP) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);		
	}

	/*
	 * Magnifying and minifying properties
	*/

	if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_NEAREST) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_BILINEAR) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);		
	}

	/*
	 * Generate mipmaps
	 *
	 * TODO: Extend this to create mipmaps with multiple filters.
	*/

	if (texture->GenerateMipmaps () && !texture->HasMipmaps ()) {
		GL::GenerateMipmap (GL_TEXTURE_2D);
	}

	/*
	 * Unbind current texture index
	*/

	GL::BindTexture (GL_TEXTURE_2D, 0);

	/*
	 * Update texture
	*/

	texture->SetGPUIndex (gpuIndex);
}