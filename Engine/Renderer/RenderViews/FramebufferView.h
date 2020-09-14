#ifndef FRAMEBUFFERVIEW_H
#define FRAMEBUFFERVIEW_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/TextureView.h"

class FramebufferView : public Object
{
protected:
	unsigned int _gpuIndex;
	std::vector<Resource<TextureView>> _textureViews;
	Resource<TextureView> _depthTextureView;

public:
	FramebufferView (unsigned int gpuIndex, const std::vector<Resource<TextureView>>& textureViews,
		const Resource<TextureView>& depthTextureView = nullptr);
	~FramebufferView ();

	void Activate ();
	void ActivateSource ();

	void SetDepthTextureView (const Resource<TextureView>& depthTextureView);

	Resource<TextureView> GetTextureView (std::size_t index) const;
	Resource<TextureView> GetDepthTextureView () const;

	std::size_t GetTextureViewCount () const;
};

#endif