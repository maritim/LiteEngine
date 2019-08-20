#include "TextureView.h"

#include "Wrappers/OpenGL/GL.h"

TextureView::TextureView () :
	_gpuIndex (0)
{

}

TextureView::~TextureView ()
{
	GL::DeleteTextures (1, &_gpuIndex);
}

void TextureView::Activate (std::size_t textureUnit)
{
	GL::ActiveTexture (GL_TEXTURE0 + textureUnit);
	GL::BindTexture (GL_TEXTURE_2D, _gpuIndex);
}

void TextureView::SetGPUIndex (unsigned int gpuIndex)
{
	_gpuIndex = gpuIndex;
}

unsigned int TextureView::GetGPUIndex () const
{
	return _gpuIndex;
}
