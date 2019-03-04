#include "LightRenderer.h"

#include "Renderer/Pipeline.h"
#include "Wrappers/OpenGL/GL.h"

LightRenderer::LightRenderer (Light* light) :
	Model3DRenderer (light->GetTransform ()),
	_light (light),
	_rvc (nullptr)
{

}

LightRenderer::~LightRenderer ()
{
	
}

void LightRenderer::Draw (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Keep render volume collection for pipeline attributes gathering
	*/

	_rvc = rvc;

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	Pipeline::SetObjectTransform (_transform);

	GL::Disable(GL_DEPTH_TEST);
	GL::BlendFunc(GL_ONE, GL_ZERO);

	GL::Disable(GL_CULL_FACE);

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Pipeline::UpdateMatrices (nullptr);
		Pipeline::SendCustomAttributes ("", GetCustomAttributes ());

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

	lightPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	lightColor.type = PipelineAttribute::AttrType::ATTR_3F;

	lightPosition.name = "lightPosition";
	lightColor.name = "lightColor";

	lightPosition.value = _transform->GetPosition ();
	lightColor.value = _light->GetColor ().ToVector3 ();

	attributes.push_back (lightPosition);
	attributes.push_back (lightColor);

	/*
	 * Attach all volume attributes to pipeline
	*/

	if (_rvc != nullptr) {
		for (RenderVolumeI* renderVolume : *_rvc) {
			std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

			attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
		}
	}

	return attributes;
}