#include "FramebufferRenderVolume.h"

#include "Renderer/RenderSystem.h"

FramebufferRenderVolume::FramebufferRenderVolume (const Resource<Framebuffer>& framebuffer) :
	_framebuffer (framebuffer)
{
	/*
	 * Load framebuffer in GPU
	*/

	_framebufferView = RenderSystem::LoadFramebuffer (_framebuffer);

	/*
	 * Creature attributes
	*/

	std::size_t index = 0;

	for_each_type (Resource<Texture>, texture, *_framebuffer) {

		PipelineAttribute textureAttribute;

		textureAttribute.type = texture->GetType () == TEXTURE_TYPE::TEXTURE_2D ?
			PipelineAttribute::AttrType::ATTR_TEXTURE_2D :
			PipelineAttribute::AttrType::ATTR_TEXTURE_3D;

		textureAttribute.name = texture->GetName ();

		textureAttribute.value.x = _framebufferView->GetTextureView (index)->GetGPUIndex ();

		_attributes.push_back (textureAttribute);

		++ index;
	}

	if (_framebuffer->GetDepthTexture () != nullptr) {

		PipelineAttribute textureAttribute;

		textureAttribute.type = _framebuffer->GetDepthTexture ()->GetType () == TEXTURE_TYPE::TEXTURE_2D ?
			PipelineAttribute::AttrType::ATTR_TEXTURE_2D :
			PipelineAttribute::AttrType::ATTR_TEXTURE_3D;

		textureAttribute.name = _framebuffer->GetDepthTexture ()->GetName ();

		textureAttribute.value.x = _framebufferView->GetDepthTextureView ()->GetGPUIndex ();

		_attributes.push_back (textureAttribute);
	}
}

const std::vector<PipelineAttribute>& FramebufferRenderVolume::GetCustomAttributes () const
{
	return _attributes;
}

Resource<Framebuffer>& FramebufferRenderVolume::GetFramebuffer ()
{
	return _framebuffer;
}

Resource<FramebufferView>& FramebufferRenderVolume::GetFramebufferView ()
{
	return _framebufferView;
}
