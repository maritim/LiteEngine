#include "LightRenderer.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"
#include "Wrappers/OpenGL/GL.h"
#include "Systems/Window/Window.h"

LightRenderer::LightRenderer (Light* light) :
	Model3DRenderer (light->GetTransform ()),
	_light (light),
	_shaderName ("LIGHT")
{
	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/deferredDirVolLightFragment.glsl");
}

LightRenderer::~LightRenderer ()
{
	
}

void LightRenderer::Draw (Scene* scene, Camera* camera, GBuffer* gBuffer)
{
	Pipeline::SetObjectTransform (_transform);

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Pipeline::SetShader (ShaderManager::Instance ()->GetShader (_shaderName));

		Pipeline::UpdateMatrices (ShaderManager::Instance ()->GetShader (_shaderName));
		std::vector<PipelineAttribute> customAttributes = GetCustomAttributes ();
		Pipeline::SendCustomAttributes (_shaderName, customAttributes);

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}		
}

std::vector<PipelineAttribute> LightRenderer::GetCustomAttributes ()
{
	// Create attribute
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightPosition;
	PipelineAttribute lightColor;
	PipelineAttribute lightSpecularColor;
	PipelineAttribute screenSize;
	PipelineAttribute deferredTexture1;
	PipelineAttribute deferredTexture2;
	PipelineAttribute deferredTexture3;
	PipelineAttribute deferredTexture4;

	lightPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	lightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	lightSpecularColor.type = PipelineAttribute::AttrType::ATTR_3F;
	screenSize.type = PipelineAttribute::AttrType::ATTR_2F;
	deferredTexture1.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture2.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture3.type = PipelineAttribute::AttrType::ATTR_1I;
	deferredTexture4.type = PipelineAttribute::AttrType::ATTR_1I;

	lightPosition.name = "lightPosition";
	lightColor.name = "lightColor";
	lightSpecularColor.name = "lightSpecularColor";
	screenSize.name = "screenSize";
	deferredTexture1.name = "gPositionMap";
	deferredTexture2.name = "gNormalMap";
	deferredTexture3.name = "gDiffuseMap";
	deferredTexture4.name = "gSpecularMap";

	lightPosition.value = _transform->GetPosition ();
	lightColor.value = _light->GetColor ().ToVector3 ();
	lightSpecularColor.value = _light->GetSpecularColor ().ToVector3 ();
	screenSize.value = glm::vec3 (Window::GetWidth (), Window::GetHeight (), 0.0f);
	deferredTexture1.value.x = 0;
	deferredTexture2.value.x = 1;
	deferredTexture3.value.x = 2;
	deferredTexture4.value.x = 3;

	attributes.push_back (lightPosition);
	attributes.push_back (lightColor);
	attributes.push_back (lightSpecularColor);
	attributes.push_back (screenSize);
	attributes.push_back (deferredTexture1);
	attributes.push_back (deferredTexture2);
	attributes.push_back (deferredTexture3);
	attributes.push_back (deferredTexture4);

	return attributes;
}