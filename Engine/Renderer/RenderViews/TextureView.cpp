#include "TextureView.h"

#include "Wrappers/OpenGL/GL.h"

TextureView::TextureView () :
	_gpuIndex (0),
	_textureType (1)
{

}

TextureView::~TextureView ()
{
	GL::DeleteTextures (1, &_gpuIndex);
}

void TextureView::Activate (std::size_t textureUnit)
{
	int textureType = GL_TEXTURE_2D;

	if (_textureType == 2) {
		textureType = GL_TEXTURE_3D;
	}

	GL::ActiveTexture (GL_TEXTURE0 + textureUnit);
	GL::BindTexture (textureType, _gpuIndex);
}

void TextureView::SetGPUIndex (unsigned int gpuIndex)
{
	_gpuIndex = gpuIndex;
}

void TextureView::SetType (int textureType)
{
	_textureType = textureType;
}

unsigned int TextureView::GetGPUIndex () const
{
	return _gpuIndex;
}
