#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Texture.h"

#include <vector>

#include "Core/Math/Vector3.h"

class TextureAtlas : public Texture
{
protected:

	// Every information is kept as percent, NOT pixels
	struct TextureArea
	{
		float startX, startY;
		float width, height;
	};

protected:
	std::vector<TextureArea> _textureAreas;

public:
	using Texture::Texture;

	void AddTextureArea (float startX, float startY, float width, float height);

	virtual Vector3 TransformTexcoord (std::size_t areaIndex, const Vector3& texcoord);

	virtual std::size_t GetAreasCount () const;

	virtual Vector3 GetOffset (std::size_t areaIndex) const;
	virtual Vector3 GetSize (std::size_t areaIndex) const;
};

#endif