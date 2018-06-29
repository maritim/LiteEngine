#include "TextureAtlas.h"

#include "Core/Console/Console.h"

void TextureAtlas::AddTextureArea (float x, float y, float w, float h)
{
	TextureArea area;

	area.startX = x;
	area.startY = y;
	area.width = w;
	area.height = h;

	_textureAreas.push_back (area);
}

std::size_t TextureAtlas::GetAreasCount () const
{
	return _textureAreas.size ();
}

glm::vec2 TextureAtlas::GetOffset (std::size_t index) const
{
	if (index >= _textureAreas.size ()) {
		Console::LogWarning ("Area index exceed areas count for " + _name + ". " +
			std::to_string (_textureAreas.size ()) + " < " + std::to_string (index) +
			" Zero vector returned!");

		return glm::vec2 (0.0f);
	}

	glm::vec2 offset (_textureAreas [index].startX, _textureAreas [index].startY);

	return offset;
}

glm::vec2 TextureAtlas::GetSize (std::size_t index) const
{
	if (index >= _textureAreas.size ()) {
		Console::LogWarning ("Area index exceed areas count for " + _name + ". " +
			std::to_string (_textureAreas.size ()) + " < " + std::to_string (index) +
			" Zero vector returned!");

		return glm::vec2 (0.0f);
	}

	glm::vec2 size (_textureAreas [index].width, _textureAreas [index].height);

	return size;
}