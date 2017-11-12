#include "Texture.h"

#include <string>
#include <string.h>

#include "Core/Console/Console.h"

Texture::Texture(const std::string& name) :
	_gpuIndex (0),
	_name (name),
	_size (0, 0),
	_generateMipmaps (true),
	_mipmapLevels (1),
	_internalFormat (0),
	_pixelFormat (TEXTURE_PIXEL_FORMAT::FORMAT_RGBA8),
	_wrapMode (TEXTURE_WRAP_MODE::WRAP_REPEAT),
	_mipmapFilter (TEXTURE_MIPMAP_FILTER::MIPMAP_ANISOTROPIC),
	_compressionType (TEXTURE_COMPRESSION_TYPE::COMPRESS_NONE),
	_isDirty (false)
{
	for (std::size_t i=0;i<MAX_TEXTURE_MIPMAP_LEVEL; i++) {
		_pixels [i] = nullptr;
	}
}

Texture::~Texture()
{
	for (std::size_t i=0;i<_mipmapLevels;i++) {
		delete[] _pixels [i];
	}
}

unsigned int Texture::GetGPUIndex() const
{
	return _gpuIndex;
}

Size Texture::GetSize () const
{
	return _size;
}

std::string Texture::GetName() const
{
	return _name;
}

bool Texture::GenerateMipmaps () const
{
	return _generateMipmaps;
}

bool Texture::HasMipmaps () const
{
	return _mipmapLevels > 1;
}

std::size_t Texture::GetMipMapLevels () const
{
	return _mipmapLevels;
}

int Texture::GetInternalFormat () const
{
	return _internalFormat;
}

TEXTURE_PIXEL_FORMAT Texture::GetPixelFormat () const
{
	return _pixelFormat;
}

TEXTURE_WRAP_MODE Texture::GetWrapMode () const
{
	return _wrapMode;
}

TEXTURE_MIPMAP_FILTER Texture::GetMipmapFilter () const
{
	return _mipmapFilter;
}

TEXTURE_COMPRESSION_TYPE Texture::GetCompressionType () const
{
	return _compressionType;
}

const unsigned char* Texture::GetPixels () const
{
	return GetMipmapLevel (0);
}

const unsigned char* Texture::GetMipmapLevel (std::size_t mipmapLevel) const
{
	if (mipmapLevel >= _mipmapLevels) {
		Console::LogWarning ("Texture mipmap out of bounds!");

		return nullptr;
	}

	return _pixels [mipmapLevel];
}

void Texture::SetGPUIndex(unsigned int gpuIndex)
{
	_gpuIndex = gpuIndex;
}

void Texture::SetName(const std::string& name)
{
	_name = name;
}

void Texture::SetSize (Size size)
{
	_size = size;
	_isDirty = true;
}

void Texture::SetMipmapsGeneration (bool generate)
{
	_generateMipmaps = generate;
	_isDirty = true;
}

void Texture::SetInternalFormat (int internalFormat)
{
	_internalFormat = internalFormat;
}

void Texture::SetPixelFormat (TEXTURE_PIXEL_FORMAT pixelFormat)
{
	_pixelFormat = pixelFormat;
}

void Texture::SetWrapMode (TEXTURE_WRAP_MODE wrapMode)
{
	_wrapMode = wrapMode;
	_isDirty = true;
}

void Texture::SetMipmapFilter (TEXTURE_MIPMAP_FILTER mipmapFilter)
{
	_mipmapFilter = mipmapFilter;
	_isDirty = true;
}

void Texture::SetCompressionType (TEXTURE_COMPRESSION_TYPE compressionType)
{
	_compressionType = compressionType;
	_isDirty = true;
}

void Texture::SetPixels (const unsigned char* pixels, std::size_t length)
{
	SetMipmapLevel (pixels, 0, length);
}

void Texture::SetMipmapLevel (const unsigned char* pixels, std::size_t mipmapLevel, std::size_t length)
{
	if (mipmapLevel >= MAX_TEXTURE_MIPMAP_LEVEL) {
		return ;
	}

	if (_pixels [mipmapLevel] != nullptr) {
		delete[] _pixels [mipmapLevel];
	}

	_pixels [mipmapLevel] = new unsigned char [length];
	memcpy (_pixels [mipmapLevel], pixels, length);
}
