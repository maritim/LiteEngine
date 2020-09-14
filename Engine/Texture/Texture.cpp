#include "Texture.h"

#include <string>
#include <string.h>

#include "Core/Console/Console.h"

Texture::Texture(const std::string& name) :
	_name (name),
	_type (TEXTURE_TYPE::TEXTURE_2D),
	_size (0, 0),
	_generateMipmap (true),
	_mipmapLevels (1),
	_sizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGBA8),
	_internalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGBA),
	_channelType (TEXTURE_CHANNEL_TYPE::CHANNEL_UNSIGNED_BYTE),
	_wrapMode (TEXTURE_WRAP_MODE::WRAP_REPEAT),
	_minFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR_MIPMAP_LINEAR),
	_magFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR),
	_compressionType (TEXTURE_COMPRESSION_TYPE::COMPRESS_NONE),
	_anisotropicFiltering (true),
	_borderColor (Color::Black),
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

void Texture::SetName(const std::string& name)
{
	_name = name;
}

void Texture::SetType (TEXTURE_TYPE type)
{
	_type = type;
}

void Texture::SetSize (Size size)
{
	_size = size;
	_isDirty = true;
}

void Texture::SetMipmapGeneration (bool generateMipmap)
{
	_generateMipmap = generateMipmap;
	_isDirty = true;
}

void Texture::SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT sizedInternalFormat)
{
	_sizedInternalFormat = sizedInternalFormat;
}

void Texture::SetInternalFormat (TEXTURE_INTERNAL_FORMAT internalFormat)
{
	_internalFormat = internalFormat;
}

void Texture::SetChannelType (TEXTURE_CHANNEL_TYPE channelType)
{
	_channelType = channelType;
}

void Texture::SetWrapMode (TEXTURE_WRAP_MODE wrapMode)
{
	_wrapMode = wrapMode;
	_isDirty = true;
}

void Texture::SetMinFilter (TEXTURE_FILTER_MODE minFilter)
{
	_minFilter = minFilter;
	_isDirty = true;
}

void Texture::SetMagFilter (TEXTURE_FILTER_MODE magFilter)
{
	_magFilter = magFilter;
	_isDirty = true;
}

void Texture::SetAnisotropicFiltering (bool anisotropicFiltering)
{
	_anisotropicFiltering = anisotropicFiltering;
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

void Texture::AddMipmapLevel (const unsigned char* pixels, std::size_t length)
{
	if (_mipmapLevels + 1 >= MAX_TEXTURE_MIPMAP_LEVEL) {
		Console::LogWarning ("Texture mipmap index exceed max mipmaps level. \
			You are searching for " + std::to_string (_mipmapLevels + 1) +
			" and the size is " + std::to_string (MAX_TEXTURE_MIPMAP_LEVEL));

		return;
	}

	_mipmapLevels ++;

	_pixels [_mipmapLevels] = new unsigned char [length];
	memcpy (_pixels [_mipmapLevels], pixels, length);
}

void Texture::SetMipmapLevel (const unsigned char* pixels, std::size_t mipmapLevel, std::size_t length)
{
	if (mipmapLevel >= _mipmapLevels) {
		Console::LogWarning ("Texture mipmap index exceed mipmaps count. \
			You are searching for " + std::to_string (mipmapLevel) +
			" and the size is " + std::to_string (_mipmapLevels));

		return;
	}

	if (_pixels [mipmapLevel] != nullptr) {
		delete[] _pixels [mipmapLevel];
	}

	_pixels [mipmapLevel] = new unsigned char [length];
	memcpy (_pixels [mipmapLevel], pixels, length);
}

void Texture::SetBorderColor (const Color& borderColor)
{
	_borderColor = borderColor;
}

const std::string& Texture::GetName() const
{
	return _name;
}

TEXTURE_TYPE Texture::GetType () const
{
	return _type;
}

Size Texture::GetSize () const
{
	return _size;
}

bool Texture::GenerateMipmap () const
{
	return _generateMipmap;
}

bool Texture::HasMipmap () const
{
	return _mipmapLevels > 1;
}

std::size_t Texture::GetMipMapLevels () const
{
	return _mipmapLevels;
}

TEXTURE_SIZED_INTERNAL_FORMAT Texture::GetSizedInternalFormat () const
{
	return _sizedInternalFormat;
}

TEXTURE_INTERNAL_FORMAT Texture::GetInternalFormat () const
{
	return _internalFormat;
}

TEXTURE_CHANNEL_TYPE Texture::GetChannelType () const
{
	return _channelType;
}

TEXTURE_WRAP_MODE Texture::GetWrapMode () const
{
	return _wrapMode;
}

TEXTURE_FILTER_MODE Texture::GetMinFilter () const
{
	return _minFilter;
}

TEXTURE_FILTER_MODE Texture::GetMagFilter () const
{
	return _magFilter;
}

bool Texture::HasAnisotropicFiltering () const
{
	return _anisotropicFiltering;
}

TEXTURE_COMPRESSION_TYPE Texture::GetCompressionType () const
{
	return _compressionType;
}

unsigned char* Texture::GetPixels () const
{
	return GetMipmapLevel (0);
}

unsigned char* Texture::GetMipmapLevel (std::size_t mipmapLevel) const
{
	if (mipmapLevel >= _mipmapLevels) {
		Console::LogWarning ("Texture mipmap index exceed mipmaps count. \
			You are searching for " + std::to_string (mipmapLevel) +
			" and the size is " + std::to_string (_mipmapLevels));

		return nullptr;
	}

	return _pixels [mipmapLevel];
}

const Color& Texture::GetBorderColor () const
{
	return _borderColor;
}
