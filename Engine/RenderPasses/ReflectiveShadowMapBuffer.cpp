#include "ReflectiveShadowMapBuffer.h"

ReflectiveShadowMapBuffer::ReflectiveShadowMapBuffer () :
	MultipleRenderTargetsVolume (RSMBUFFER_NUM_TEXTURES)
{

}

void ReflectiveShadowMapBuffer::BindForReading ()
{
	/*
	 * Bind every color buffer for reading
	*/

	for (std::size_t index = 0;index < m_texturesCount;index ++) {
		GL::ActiveTexture (GL_TEXTURE4 + index);
		GL::BindTexture (GL_TEXTURE_2D, m_textures [index]);
	}
}