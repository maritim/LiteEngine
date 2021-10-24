#ifndef FRAMEBUFFERRENDERVOLUME_H
#define FRAMEBUFFERRENDERVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Framebuffer/Framebuffer.h"
#include "Renderer/RenderViews/FramebufferView.h"

class ENGINE_API FramebufferRenderVolume : public RenderVolumeI
{
protected:
	Resource<Framebuffer> _framebuffer;
	Resource<FramebufferView> _framebufferView;

	std::vector<PipelineAttribute> _attributes;

public:
	FramebufferRenderVolume (const Resource<Framebuffer>& framebuffer);

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;

	Resource<Framebuffer>& GetFramebuffer ();
	Resource<FramebufferView>& GetFramebufferView ();
};

#endif