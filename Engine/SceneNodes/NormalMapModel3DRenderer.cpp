#include "NormalMapModel3DRenderer.h"

#include <glm/vec3.hpp>
#include <vector>

#include "Renderer/Pipeline.h"

#include "Managers/ShaderManager.h"
#include "Managers/MaterialManager.h"

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Extensions/VectorExtend.h"

NormalMapVertexData::NormalMapVertexData () : VertexData ()
{
	for (std::size_t i = 0; i < 3; i++) {
		tangent [i] = 0;
	}
}

void NormalMapModel3DRenderer::Draw ()
{
	Pipeline::SetObjectTransform (_transform);

	for (std::size_t i = 0; i<_drawableObjects.size (); i++) {
		Material* mat = MaterialManager::Instance ()->GetMaterial (_drawableObjects [i].MAT_NAME);

		Pipeline::SendMaterial (mat);

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray (_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}
}

// For the moment I clone the vertex/normal/texture tuple for every one
// Maybe will be a good idea to delete the duplicates, low priority TODO:
BufferObject NormalMapModel3DRenderer::ProcessPolygonGroup (const Resource<Model>& model, PolygonGroup* polyGroup)
{
	std::vector<NormalMapVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	std::size_t polygonIndex = 0;

	for (Polygon* polygon : *polyGroup) {
		for (std::size_t j = 0; j<polygon->VertexCount (); j++) {
			NormalMapVertexData vertexData;

			glm::vec3 position = model->GetVertex (polygon->GetVertex (j));
			vertexData.position [0] = position.x;
			vertexData.position [1] = position.y;
			vertexData.position [2] = position.z;

			if (polygon->HaveNormals ()) {
				glm::vec3 normal = model->GetNormal (polygon->GetNormal (j));
				vertexData.normal [0] = normal.x;
				vertexData.normal [1] = normal.y;
				vertexData.normal [2] = normal.z;
			}

			//glm::vec3 normal = CalculateNormal (model, polygon);
			//vertexData.normal [0] = normal.x;
			//vertexData.normal [1] = normal.y;
			//vertexData.normal [2] = normal.z;

			if (model->HaveUV ()) {
				glm::vec2 texcoord = model->GetTexcoord (polygon->GetTexcoord (j));
				vertexData.texcoord [0] = texcoord.x;
				vertexData.texcoord [1] = texcoord.y;
			}

			if (model->HaveUV ()) {
				glm::vec3 tangent = CalculateTangent (model, polygon);

				vertexData.tangent [0] = tangent.x;
				vertexData.tangent [1] = tangent.y;
				vertexData.tangent [2] = tangent.z;
			}

			vertexBuffer.push_back (vertexData);
		}

		indexBuffer.push_back (3 * (unsigned int) polygonIndex);
		indexBuffer.push_back (3 * (unsigned int) polygonIndex + 1);
		indexBuffer.push_back (3 * (unsigned int) polygonIndex + 2);

		++ polygonIndex;
	}

	BufferObject bufObj = BindVertexData (vertexBuffer, indexBuffer);
	bufObj.MAT_NAME = polyGroup->GetMaterialName ();

	return bufObj;
}

BufferObject NormalMapModel3DRenderer::BindVertexData (const std::vector<NormalMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays (1, &VAO);
	GL::BindVertexArray (VAO);

	//creeaza vbo
	GL::GenBuffers (1, &VBO);
	GL::BindBuffer (GL_ARRAY_BUFFER, VBO);
	GL::BufferData (GL_ARRAY_BUFFER, sizeof (NormalMapVertexData)*vBuf.size (), &vBuf [0], GL_STATIC_DRAW);

	//creeaza ibo
	GL::GenBuffers (1, &IBO);
	GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (unsigned int)*iBuf.size (), &iBuf [0], GL_STATIC_DRAW);

	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray (0);																			//activare pipe 0
	GL::VertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) 0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray (1);																			//activare pipe 1
	GL::VertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) (sizeof (float) * 3));	//trimite normale pe pipe 1
	GL::EnableVertexAttribArray (2);																			//activare pipe 2
	GL::VertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) (sizeof (float) * 6));	//trimite texcoorduri pe pipe 2
	GL::EnableVertexAttribArray (3);																			//activare pipe 2
	GL::VertexAttribPointer (3, 3, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) (sizeof (float) * 8));	//trimite texcoorduri pe pipe 2

	//numar de indecsi
	std::size_t indexCount = iBuf.size ();

	BufferObject bufferObject;
	bufferObject.VAO_INDEX = VAO;
	bufferObject.VBO_INDEX = VBO;
	bufferObject.IBO_INDEX = IBO;
	bufferObject.VBO_INSTANCE_INDEX = 0;
	bufferObject.INDEX_COUNT = indexCount;

	return bufferObject;
}

glm::vec3 NormalMapModel3DRenderer::CalculateNormal (const Resource<Model>& model, Polygon* polygon)
{
	glm::vec3 normal = Extensions::VectorExtend::Cross(
		model->GetVertex (polygon->GetVertex (0)),
		model->GetVertex (polygon->GetVertex (1)),
		model->GetVertex (polygon->GetVertex (2))
	);

	return glm::normalize (normal);
}

/*
 * Thanks to: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
 *
 * Calculate vertex tangent based on explanation from the link above;
*/

glm::vec3 NormalMapModel3DRenderer::CalculateTangent (const Resource<Model>& model, Polygon* poly)
{
	/*
	 * Tangents are generated only when texcoords are present
	*/

	if (!model->HaveUV ()) {
		return glm::vec3 (0.0f);
	}

	/*
	 * Get vertices
	*/

	glm::vec3 v0 = model->GetVertex(poly->GetVertex (0));
	glm::vec3 v1 = model->GetVertex(poly->GetVertex (1));
	glm::vec3 v2 = model->GetVertex(poly->GetVertex (2));

	/*
	 * Get texcoords
	*/

	glm::vec2 uv0 = model->GetTexcoord(poly->GetTexcoord (0));
	glm::vec2 uv1 = model->GetTexcoord (poly->GetTexcoord (1));
	glm::vec2 uv2 = model->GetTexcoord (poly->GetTexcoord (2));

	/*
	 * Edges of the triangle : postion delta
	*/

	glm::vec3 deltaPos1 = v1 - v0;
	glm::vec3 deltaPos2 = v2 - v0;

	/*
	 * UV delta
	*/

	glm::vec2 deltaUV1 = uv1 - uv0;
	glm::vec2 deltaUV2 = uv2 - uv0;

	/*
	 * "Here is the algorithm : if we note deltaPos1 and deltaPos2 two edges of
	 * our triangle, and deltaUV1 and deltaUV2 the corresponding differences in
	 * UVs, we can express our problem with the following equation :
	 *
	 * deltaPos1 = deltaUV1.x * T + deltaUV1.y * B
	 * deltaPos2 = deltaUV2.x * T + deltaUV2.y * B
	 *
	 * Just solve this system for T and B, and you have your vectors !
	*/

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

	/*
	 * Return the tangent
	*/

	return glm::normalize (tangent);
}
