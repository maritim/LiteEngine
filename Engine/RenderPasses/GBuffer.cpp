#include "GBuffer.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Window/Window.h"

GBuffer::GBuffer () :
	MultipleRenderTargetsVolume (GBUFFER_NUM_TEXTURES)
{

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

	deferredTexture1.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture2.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture3.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture4.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture5.type = PipelineAttribute::AttrType::ATTR_1I;
	screenSize.type = PipelineAttribute::AttrType::ATTR_2F;

	deferredTexture1.name = "gPositionMap";
	deferredTexture2.name = "gNormalMap";
	deferredTexture3.name = "gDiffuseMap";
	deferredTexture4.name = "gSpecularMap";
	deferredTexture5.name = "gLightMap";
	screenSize.name = "screenSize";

	deferredTexture1.value.x = 0;
	deferredTexture2.value.x = 1;
	deferredTexture3.value.x = 2;
	deferredTexture4.value.x = 3;
	deferredTexture5.value.x = 4;
	screenSize.value = glm::vec3 (Window::GetWidth (), Window::GetHeight (), 0.0f);

	attributes.push_back (deferredTexture1);
	attributes.push_back (deferredTexture2);
	attributes.push_back (deferredTexture3);
	attributes.push_back (deferredTexture4);
	attributes.push_back (deferredTexture5);
	attributes.push_back (screenSize);

	return attributes;
}
