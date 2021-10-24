#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Texture.h"

#include <glm/vec2.hpp>
#include <vector>

class ENGINE_API TextureAtlas : public Texture
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

	virtual std::size_t GetAreasCount () const;

	virtual glm::vec2 GetOffset (std::size_t areaIndex) const;
	virtual glm::vec2 GetSize (std::size_t areaIndex) const;
};

#endif