#include "LightMapModel3DRenderer.h"

#include <glm/vec3.hpp>
#include <vector>

#include "Mesh/LightMapModel.h"

#include "Renderer/Pipeline.h"

#include "Managers/ShaderManager.h"
#include "Managers/MaterialManager.h"

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Extensions/VectorExtend.h"

LightMapVertexData::LightMapVertexData () : VertexData ()
{
	for (std::size_t i = 0; i < 2; i++) {
		lmTexcoord [i] = 0;
	}
}

// For the moment I clone the vertex/normal/texture tuple for every one
// Maybe will be a good idea to delete the duplicates, low priority TODO:
BufferObject LightMapModel3DRenderer::ProcessPolygonGroup (const Resource<Model>& model, PolygonGroup* polyGroup)
{
	const LightMapModel* lmModel = dynamic_cast<const LightMapModel*> (&*model);

	std::vector<LightMapVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	std::size_t polygonIndex = 0;

	for (Polygon* polygon : *polyGroup) {
		for (std::size_t j = 0; j<polygon->VertexCount (); j++) {
			LightMapVertexData vertexData;

			glm::vec3 position = lmModel->GetVertex (polygon->GetVertex (j));
			vertexData.position [0] = position.x;
			vertexData.position [1] = position.y;
			vertexData.position [2] = position.z;

			if (polygon->HaveNormals ()) {
				glm::vec3 normal = lmModel->GetNormal (polygon->GetNormal (j));
				vertexData.normal [0] = normal.x;
				vertexData.normal [1] = normal.y;
				vertexData.normal [2] = normal.z;
			}

			if (lmModel->HaveUV ()) {
				glm::vec2 texcoord = lmModel->GetTexcoord (polygon->GetTexcoord (j));
				vertexData.texcoord [0] = texcoord.x;
				vertexData.texcoord [1] = texcoord.y;
			}

			if (lmModel->HaveLightMapUV ()) {
				glm::vec2 lmTexcoord = lmModel->GetLightMapTexcoord (polygon->GetTexcoord (j));
				vertexData.lmTexcoord [0] = lmTexcoord.x;
				vertexData.lmTexcoord [1] = lmTexcoord.y;
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

BufferObject LightMapModel3DRenderer::BindVertexData (const std::vector<LightMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays (1, &VAO);
	GL::BindVertexArray (VAO);

	//creeaza vbo
	GL::GenBuffers (1, &VBO);
	GL::BindBuffer (GL_ARRAY_BUFFER, VBO);
	GL::BufferData (GL_ARRAY_BUFFER, sizeof (LightMapVertexData)*vBuf.size (), &vBuf [0], GL_STATIC_DRAW);

	//creeaza ibo
	GL::GenBuffers (1, &IBO);
	GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (unsigned int)*iBuf.size (), &iBuf [0], GL_STATIC_DRAW);

	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray (0);																			//activare pipe 0
	GL::VertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) 0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray (1);																			//activare pipe 1
	GL::VertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) (sizeof (float) * 3));	//trimite normale pe pipe 1
	GL::EnableVertexAttribArray (2);																			//activare pipe 2
	GL::VertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) (sizeof (float) * 6));	//trimite texcoorduri pe pipe 2
	GL::EnableVertexAttribArray (3);																			//activare pipe 2
	GL::VertexAttribPointer (3, 2, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) (sizeof (float) * 8));	//trimite texcoorduri pe pipe 2

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
