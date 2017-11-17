#include "ReflectiveShadowMapBuffer.h"

ReflectiveShadowMapBuffer::ReflectiveShadowMapBuffer () :
	MultipleRenderTargetsVolume (RSMBUFFER_NUM_TEXTURES)
{

}

void ReflectiveShadowMapBuffer::BindForReading ()
{
	/*
	 * Bind depth buffer for reading
	*/

	GL::ActiveTexture (GL_TEXTURE4);
	GL::BindTexture (GL_TEXTURE_2D, m_depthTexture);

	/*
	 * Bind every color buffer for reading
	*/

	for (std::size_t index = 0;index < m_texturesCount;index ++) {
		GL::ActiveTexture (GL_TEXTURE5 + index);
		GL::BindTexture (GL_TEXTURE_2D, m_textures [index]);
	}
}