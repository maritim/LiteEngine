#ifndef TEXTURERENDERVOLUME_H
#define TEXTURERENDERVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include <vector>

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Texture/Texture.h"
#include "Renderer/RenderViews/TextureView.h"

#include "Core/Iteration/MultipleContainer.h"

class ENGINE_API TextureRenderVolume : public RenderVolumeI
{
protected:
	std::vector<Resource<Texture>> _textures;
	std::vector<Resource<TextureView>> _textureViews;

	std::vector<PipelineAttribute> _attributes;

public:
	TextureRenderVolume (const std::vector<Resource<Texture>>& textures);
	TextureRenderVolume (const Resource<Texture>& texture);

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	Resource<Texture> GetTexture (std::size_t index) const;
	Resource<TextureView> GetTextureView (std::size_t index) const;

	MULTIPLE_CONTAINER_TEMPLATE (vector)
};

MULTIPLE_CONTAINER_SPECIALIZATION (vector, Resource<Texture>, TextureRenderVolume, _textures);
MULTIPLE_CONTAINER_SPECIALIZATION (vector, Resource<TextureView>, TextureRenderVolume, _textureViews);

#endif