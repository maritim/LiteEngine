#include "GBuffer.h"

#include "Wrappers/OpenGL/GL.h"

GBuffer::GBuffer () :
	MultipleRenderTargetsVolume (GBUFFER_NUM_TEXTURES),
	_depthTextureView (0)
{

}

bool GBuffer::Init (const glm::ivec2& size)
{
	if (!MultipleRenderTargetsVolume::Init (size)) {
		return false;
	}

	GL::GenTextures (1, &_depthTextureView);
	GL::TextureView (_depthTextureView, GL_TEXTURE_2D, m_depthTexture,
		GL_DEPTH24_STENCIL8, 0, 1, 0, 1);

	return true;
}

std::vector<PipelineAttribute> GBuffer::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute deferredTexture1;
	PipelineAttribute deferredTexture2;
	PipelineAttribute deferredTexture3;
	PipelineAttribute deferredTexture4;
	PipelineAttribute deferredTexture5;
	PipelineAttribute screenSize;

	deferredTexture1.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	deferredTexture2.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	deferredTexture3.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	deferredTexture4.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	deferredTexture5.type = PipelineAttribute::AttrType::ATTR_TEXTURE_VIEW_DEPTH;
	screenSize.type = PipelineAttribute::AttrType::ATTR_2F;

	deferredTexture1.name = "gPositionMap";
	deferredTexture2.name = "gNormalMap";
	deferredTexture3.name = "gDiffuseMap";
	deferredTexture4.name = "gSpecularMap";
	deferredTexture5.name = "gDepthMap";
	screenSize.name = "screenSize";

	deferredTexture1.value.x = m_textures [0];
	deferredTexture2.value.x = m_textures [1];
	deferredTexture3.value.x = m_textures [2];
	deferredTexture4.value.x = m_textures [3];
	deferredTexture5.value.x = _depthTextureView;
	screenSize.value = glm::vec3 (_size, 0.0f);

	attributes.push_back (deferredTexture1);
	attributes.push_back (deferredTexture2);
	attributes.push_back (deferredTexture3);
	attributes.push_back (deferredTexture4);
	attributes.push_back (deferredTexture5);
	attributes.push_back (screenSize);

	return attributes;
}
