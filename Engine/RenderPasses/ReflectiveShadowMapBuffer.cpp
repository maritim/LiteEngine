#include "ReflectiveShadowMapBuffer.h"

ReflectiveShadowMapBuffer::ReflectiveShadowMapBuffer () :
	MultipleRenderTargetsVolume (RSMBUFFER_NUM_TEXTURES)
{

}

bool ReflectiveShadowMapBuffer::Init (const glm::ivec2& size)
{
	/*
	 * Use default initiator
	*/

	if (!MultipleRenderTargetsVolume::Init (size)) {
		return false;
	}

	/*
	 * Change map properties
	*/

	for (std::size_t index = 0; index < m_texturesCount; index++) {
		GL::BindTexture(GL_TEXTURE_2D, m_textures[index]);

		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	return true;
}

void ReflectiveShadowMapBuffer::BindForReading ()
{
	/*
	 * Bind every color buffer for reading
	*/

	for (std::size_t index = 0;index < m_texturesCount;index ++) {
		GL::ActiveTexture (GL_TEXTURE8 + index);
		GL::BindTexture (GL_TEXTURE_2D, m_textures [index]);
	}
}