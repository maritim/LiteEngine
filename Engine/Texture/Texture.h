#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Interfaces/Object.h"

#include "TextureMode.h"
#include "Utils/Color/Color.h"

#define MAX_TEXTURE_MIPMAP_LEVEL 16

// TODO: Extend this
class ENGINE_API Texture : public Object
{
protected:
	std::string _name;

	unsigned char *_pixels[MAX_TEXTURE_MIPMAP_LEVEL];

	Size _size;
	bool _generateMipmap;
	std::size_t _mipmapLevels;
	TEXTURE_SIZED_INTERNAL_FORMAT _sizedInternalFormat;
	TEXTURE_INTERNAL_FORMAT _internalFormat;
	TEXTURE_CHANNEL_TYPE _channelType;
	TEXTURE_WRAP_MODE _wrapMode;
	TEXTURE_FILTER_MODE _minFilter;
	TEXTURE_FILTER_MODE _magFilter;
	TEXTURE_COMPRESSION_TYPE _compressionType;
	bool _anisotropicFiltering;
	Color _borderColor;

	bool _isDirty;

public:
	Texture(const std::string& name);
	virtual ~Texture();

	void SetName(const std::string& name);
	void SetSize (Size size);
	void SetMipmapGeneration (bool generate);
	void SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT internalFormat);
	void SetInternalFormat (TEXTURE_INTERNAL_FORMAT pixelFormat);
	void SetChannelType (TEXTURE_CHANNEL_TYPE channelType);
	void SetWrapMode (TEXTURE_WRAP_MODE wrapMode);
	void SetMinFilter (TEXTURE_FILTER_MODE minFilter);
	void SetMagFilter (TEXTURE_FILTER_MODE magFilter);
	void SetAnisotropicFiltering (bool anisotropicFiltering);
	void SetCompressionType (TEXTURE_COMPRESSION_TYPE compressionType);
	void SetPixels (const unsigned char* pixels, std::size_t length);
	void AddMipmapLevel (const unsigned char* pixels, std::size_t length);
	void SetMipmapLevel (const unsigned char* pixels, std::size_t mipmapLevel, std::size_t length);
	void SetBorderColor (const Color& color);

	std::string GetName() const;
	Size GetSize () const;
	bool GenerateMipmap () const;
	bool HasMipmap () const;
	std::size_t GetMipMapLevels () const;
	TEXTURE_SIZED_INTERNAL_FORMAT GetSizedInternalFormat () const;
	TEXTURE_INTERNAL_FORMAT GetInternalFormat () const;
	TEXTURE_CHANNEL_TYPE GetChannelType () const;
	TEXTURE_WRAP_MODE GetWrapMode () const;
	TEXTURE_FILTER_MODE GetMinFilter () const;
	TEXTURE_FILTER_MODE GetMagFilter () const;
	bool HasAnisotropicFiltering () const;
	TEXTURE_COMPRESSION_TYPE GetCompressionType () const;
	// TODO: extend PixelAt
	unsigned char* GetPixels () const;
	unsigned char* GetMipmapLevel (std::size_t mipmapLevel) const;
	const Color& GetBorderColor () const;
};

#endif