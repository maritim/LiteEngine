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

glm::vec3 TextureAtlas::TransformTexcoord (std::size_t areaIndex, const glm::vec3& texcoord)
{
	if (areaIndex >= _textureAreas.size ()) {
		Console::LogWarning ("Area index exceed areas count for " + _name + ". " +
			std::to_string (_textureAreas.size ()) + " < " + std::to_string (areaIndex) +
			" Original texcoord returned!");

		return texcoord;
	}

	glm::vec3 result (glm::vec3 (0.0f));
	TextureArea& texArea = _textureAreas [areaIndex];

	result.x = texArea.startX + texcoord.x * texArea.width;
	result.y = texArea.startY + texcoord.y * texArea.height;

	return result;
}

std::size_t TextureAtlas::GetAreasCount () const
{
	return _textureAreas.size ();
}

glm::vec3 TextureAtlas::GetOffset (std::size_t index) const
{
	if (index >= _textureAreas.size ()) {
		Console::LogWarning ("Area index exceed areas count for " + _name + ". " +
			std::to_string (_textureAreas.size ()) + " < " + std::to_string (index) +
			" Zero vector returned!");

		return glm::vec3 (0.0f);
	}

	glm::vec3 offset (_textureAreas [index].startX, _textureAreas [index].startY, 0.0f);

	return offset;
}

glm::vec3 TextureAtlas::GetSize (std::size_t index) const
{
	if (index >= _textureAreas.size ()) {
		Console::LogWarning ("Area index exceed areas count for " + _name + ". " +
			std::to_string (_textureAreas.size ()) + " < " + std::to_string (index) +
			" Zero vector returned!");

		return glm::vec3 (0.0f);
	}

	glm::vec3 size (_textureAreas [index].width, _textureAreas [index].height, 0.0f);

	return size;
}