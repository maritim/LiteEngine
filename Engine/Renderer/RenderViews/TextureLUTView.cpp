#include "TextureLUTView.h"

#include "Wrappers/OpenGL/GL.h"

void TextureLUTView::Activate (std::size_t textureUnit)
{
	GL::ActiveTexture (GL_TEXTURE0 + textureUnit);
	GL::BindTexture (GL_TEXTURE_3D, _gpuIndex);
}