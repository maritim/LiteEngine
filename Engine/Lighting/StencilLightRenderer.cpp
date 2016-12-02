#include "StencilLightRenderer.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"
#include "Wrappers/OpenGL/GL.h"

StencilLightRenderer::StencilLightRenderer (Light* light) :
	LightRenderer (light),
	_stencilShaderName ("STENCIL_LIGHT")
{
	ShaderManager::Instance ()->AddShader (_stencilShaderName,
		"Assets/Shaders/deferredStencilVolLightVertex.glsl",
		"Assets/Shaders/deferredStencilVolLightFragment.glsl");
}

void StencilLightRenderer::StencilPass ()
{
	Pipeline::SetObjectTransform (_transform);

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Pipeline::SetShader (ShaderManager::Instance ()->GetShader (_stencilShaderName));

		Pipeline::UpdateMatrices (ShaderManager::Instance ()->GetShader (_stencilShaderName));

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}
}