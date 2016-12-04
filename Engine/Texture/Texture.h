#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Interfaces/Object.h"

#include "TextureMode.h"

#define MAX_MIPMAP_LEVEL 16

// TODO: Extend this
class Texture : public Object
{
protected:
	unsigned int _gpuIndex;
	std::string _name;

	unsigned char *_pixels[MAX_MIPMAP_LEVEL];

	Size _size;
	bool _generateMipmaps;
	std::size_t _mipmapLevels;
	TEXTURE_WRAP_MODE _wrapMode;
	TEXTURE_MIPMAP_FILTER _mipmapFilter;
	TEXTURE_COMPRESSION_TYPE _compressionType;

	bool _isDirty;

public:
	Texture(const std::string& name);
	virtual ~Texture();

	unsigned int GetGPUIndex() const;
	std::string GetName() const;

	Size GetSize () const;
	bool GenerateMipmaps () const;
	bool HasMipmaps () const;
	TEXTURE_WRAP_MODE GetWrapMode () const;
	TEXTURE_MIPMAP_FILTER GetMipmapFilter () const;
	TEXTURE_COMPRESSION_TYPE GetCompressionType () const;
	const unsigned char* GetPixels () const;
	const unsigned char* GetMipmapLevel (std::size_t mipmapLevel) const;

	void SetGPUIndex(unsigned int gpuIndex);
	void SetName(const std::string& name);

	void SetSize (Size size);
	void SetMipmapsGeneration (bool generate);
	void SetWrapMode (TEXTURE_WRAP_MODE wrapMode);
	void SetMipmapFilter (TEXTURE_MIPMAP_FILTER mipmapFilter);
	void SetCompressionType (TEXTURE_COMPRESSION_TYPE compressionType);
	void SetPixels (const unsigned char* pixels, std::size_t length);
	void SetMipmapLevel (const unsigned char* pixels, std::size_t mipmapLevel, std::size_t length);
};

#endif