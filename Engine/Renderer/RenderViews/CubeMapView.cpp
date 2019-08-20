#include "CubeMapView.h"

#include "Wrappers/OpenGL/GL.h"

void CubeMapView::Activate (std::size_t textureUnit)
{
	GL::ActiveTexture (GL_TEXTURE0 + textureUnit);
	GL::BindTexture (GL_TEXTURE_CUBE_MAP, _gpuIndex);
}