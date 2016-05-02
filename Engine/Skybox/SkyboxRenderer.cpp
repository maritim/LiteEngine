#include <GL/glew.h>

#include "SkyboxRenderer.h"

#include <string>
#include <vector>

#include "Utils/Primitives/Primitive.h"
#include "Renderer/Pipeline.h"
#include "Renderer/PipelineAttribute.h"
#include "Mesh/Model.h"

#include "Wrappers/OpenGL/GL.h"

#include "Managers/ShaderManager.h"
#include "Shader/Shader.h"

SkyboxRenderer::SkyboxRenderer (CubeMap*& map, Color* tint, float *bt, std::string shn) :
	Model3DRenderer (NULL),
	_cubeMap (map),
	_tintColor (tint),
	_brightness (bt),
	_shaderName (shn)
{
	Model* cube = Primitive::Instance ()->Create (Primitive::Type::CUBE);
	Attach (cube);
	delete cube;
}

void SkyboxRenderer::Draw ()
{
	GL::Disable(GL_DEPTH_TEST);
	GL::Enable(GL_TEXTURE_2D);

	bool cull;
	GL::IsEnabled (GL_CULL_FACE, &cull);
	GL::Disable(GL_CULL_FACE);

	GL::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = ShaderManager::Instance ()->GetShader (_shaderName);
	Pipeline::SetShader (shader);

	Pipeline::SetObjectTransform (Transform::Default ());

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Pipeline::UpdateMatrices (shader);

		ManageCustomAttributes ();

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements(GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}

	GL::Enable(GL_DEPTH_TEST);
	GL::Disable(GL_TEXTURE_2D);

	if (cull) {
		GL::Enable (GL_CULL_FACE);
	}
}

void SkyboxRenderer::ManageCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute uniformCubeMap, uniformTintColor, uniformBrightness;

	uniformCubeMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_CUBE;
	uniformTintColor.type = PipelineAttribute::AttrType::ATTR_3F;
	uniformBrightness.type = PipelineAttribute::AttrType::ATTR_1F;

	uniformCubeMap.name = "cubeMap";
	uniformTintColor.name = "tintColor";
	uniformBrightness.name = "brightness";

	uniformCubeMap.value.x = _cubeMap->GetGPUIndex ();
	uniformTintColor.value = _tintColor->ToVector3 ();
	uniformBrightness.value.x = *_brightness;

	attributes.push_back (uniformCubeMap);
	attributes.push_back (uniformTintColor);
	attributes.push_back (uniformBrightness);

	Pipeline::SendCustomAttributes (_shaderName, attributes);
}