#include "FramebufferView.h"
#include "FramebufferView.h"

#include "Wrappers/OpenGL/GL.h"

FramebufferView::FramebufferView (unsigned int gpuIndex,
	const std::vector<Resource<TextureView>>& textureViews,
	const Resource<TextureView>& depthTextureView) :
	_gpuIndex (gpuIndex),
	_textureViews (textureViews),
	_depthTextureView (depthTextureView)
{

}

FramebufferView::~FramebufferView ()
{
	/*
	 * Bind current framebuffer for cleaning
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _gpuIndex);

	/*
	 * Detach textures from color attachments in framebuffer
	*/

	for (std::size_t index = 0; index < _textureViews.size (); index ++) {
		GL::FramebufferTexture (GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 0, 0);
	}

	/*
	 * Detach depth texture from attachment in FBO
	*/

	GL::FramebufferTexture (GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 0, 0);

	/*
	 * Delete framebuffer
	*/

	GL::DeleteFramebuffers (1, &_gpuIndex);

	/*
	 * Restore default framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);
}

void FramebufferView::Activate ()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _gpuIndex);

	/*
	 * Enable all color attachments correspunding with color textures
	*/

	std::vector<GLenum> DrawBuffers;
	for (std::size_t index = 0; index < _textureViews.size (); index ++) {
		DrawBuffers.push_back ((GLenum)(GL_COLOR_ATTACHMENT0 + index));
	}

	GL::DrawBuffers(DrawBuffers.size (), DrawBuffers.data ());

	// GL::DrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FramebufferView::ActivateSource ()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_READ_FRAMEBUFFER, _gpuIndex);

	/*
	 * Enable all color attachments correspunding with color textures
	*/

	GL::ReadBuffer (GL_COLOR_ATTACHMENT0);
}

void FramebufferView::SetDepthTextureView (const Resource<TextureView>& depthTextureView)
{
	_depthTextureView = depthTextureView;
}

Resource<TextureView> FramebufferView::GetTextureView (std::size_t index) const
{
	if (index >= _textureViews.size ()) {
		exit (1);
		// return nullptr;
	}

	return _textureViews [index];
}

Resource<TextureView> FramebufferView::GetDepthTextureView () const
{
	return _depthTextureView;
}

std::size_t FramebufferView::GetTextureViewCount () const
{
	return _textureViews.size ();
}
