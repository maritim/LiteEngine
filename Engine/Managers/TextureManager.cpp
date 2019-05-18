#include "TextureManager.h"

#include <map>
#include <string>

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Extensions/MathExtend.h"

TextureManager::TextureManager () :
	_textures ()
{

}

TextureManager::~TextureManager ()
{

}

SPECIALIZE_SINGLETON(TextureManager)

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

/*
 * TODO: Extend this to accept external Texture settings
*/

void TextureManager::LoadInGPU (Texture* texture)
{
	unsigned int gpuIndex = 0;

	Size size (texture->GetSize ());

	GL::GenTextures(1, &gpuIndex);
	GL::BindTexture(GL_TEXTURE_2D, gpuIndex);

	/*
	 * Get Pixel Format
	*/

	int pixelFormat = GL_RGBA;

	switch (texture->GetPixelFormat ()) {
		case FORMAT_RGB8:
			pixelFormat = GL_RGB;
			break;
		case FORMAT_BGR8:
			pixelFormat = GL_BGR;
			break;
		case FORMAT_RGBA8:
			pixelFormat = GL_RGBA;
			break;
		case FORMAT_ABGR8:
			pixelFormat = GL_BGRA;
			break;
		default:
			pixelFormat = GL_RGBA;
			break;
	}

	/*
	 * Send MipMaps to GPU
	*/

	for (std::size_t i=0;i<texture->GetMipMapLevels ();i++) {
		int internalFormat = texture->GetInternalFormat ();

		if (internalFormat == 0) {
			internalFormat = GL_RGBA;
		}

		GL::TexImage2D(GL_TEXTURE_2D, i, internalFormat, size.width >> i, size.height >> i, 0, pixelFormat, GL_UNSIGNED_BYTE, texture->GetMipmapLevel (i));
	}

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
	else if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_ANISOTROPIC) {
		GLfloat maxAnisotropy;
		GL::GetFloatv (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

		maxAnisotropy = Extensions::MathExtend::Clamp (maxAnisotropy, 0.0f, 8.0f);
		GL::TexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	} 
	else if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_NONE) {
		GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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