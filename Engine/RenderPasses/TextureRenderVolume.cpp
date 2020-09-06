#include "TextureRenderVolume.h"

#include "Renderer/RenderSystem.h"

#include "Core/Console/Console.h"

TextureRenderVolume::TextureRenderVolume (const std::vector<Resource<Texture>>& textures) :
	_textures (textures)
{
	/*
	 * Creature attributes
	*/

	for (auto& texture : _textures) {

		auto textureView = RenderSystem::LoadTexture (texture);

		_textureViews.push_back (textureView);

		/*
		 * Create attributes
		*/

		PipelineAttribute textureAttribute;
		PipelineAttribute textureSize;

		textureAttribute.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
		textureSize.type = PipelineAttribute::AttrType::ATTR_2I;

		textureAttribute.name = texture->GetName ();
		textureSize.name = texture->GetName () + "Size";

		auto size = texture->GetSize ();

		textureAttribute.value.x = textureView->GetGPUIndex ();
		textureSize.value = glm::vec3 (size.width, size.height, 0);

		_attributes.push_back (textureAttribute);
		_attributes.push_back (textureSize);
	}
}

TextureRenderVolume::TextureRenderVolume (const Resource<Texture>& texture) :
	TextureRenderVolume (std::vector<Resource<Texture>> { texture })
{

}

const std::vector<PipelineAttribute>& TextureRenderVolume::GetCustomAttributes () const
{
	return _attributes;
}

Resource<Texture> TextureRenderVolume::GetTexture (std::size_t index) const
{
	if (index >= _textures.size ()) {
		Console::LogWarning ("There is no texture index " + std::to_string (index) + " in framebuffer.");

		return nullptr;
	}

	return _textures [index];
}

Resource<TextureView> TextureRenderVolume::GetTextureView (std::size_t index) const
{
	if (index >= _textureViews.size ()) {
		Console::LogWarning ("There is no texture index " + std::to_string (index) + " in framebuffer.");

		return nullptr;
	}

	return _textureViews [index];
}
