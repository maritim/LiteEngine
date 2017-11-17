#include "GBuffer.h"

#include "Wrappers/OpenGL/GL.h"

GBuffer::GBuffer () :
	MultipleRenderTargetsVolume (GBUFFER_NUM_TEXTURES)
{

}

void GBuffer::BindForGeomPass()
{
	/*
	 * Bind all textures for writing
	*/

	BindForWriting ();
} 

void GBuffer::BindForStencilPass()
{
	/*
	 * Must disable the draw buffers
	*/

	GL::DrawBuffer(GL_NONE);
}

void GBuffer::BindForLightPass()
{
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	
	GL::DrawBuffer(GL_COLOR_ATTACHMENT0 + m_finalTextureIndex);

	for (std::size_t i = 0 ; i < m_texturesCount; i++) {
		GL::ActiveTexture(GL_TEXTURE0 + i);
		GL::BindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}
