#include "Model3DRenderer.h"

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <algorithm>

#include "Renderer/Pipeline.h"

#include "Material/Material.h"
#include "Managers/MaterialManager.h"
#include "Mesh/Polygon.h"

#include "Wrappers/OpenGL/GL.h"

inline bool BufferObjectSorter::operator() (const BufferObject& object1, const BufferObject& object2)
{
    return object1.MAT_NAME < object2.MAT_NAME;
}

VertexData::VertexData ()
{
	for (std::size_t i=0;i<3;i++) {
		position [i] = normal [i] = 0;
	}

	texcoord [0] = texcoord [1] = 0;
}

Model3DRenderer::~Model3DRenderer ()
{
	Clear ();
}

void Model3DRenderer::Attach (const Resource<Model>& model)
{
	/*
	 * Clear current buffer data if exists
	*/

	Clear ();

	/*
	 * Create buffer data
	*/

	for_each_type (ObjectModel*, objModel, *model) {
		ProcessObjectModel (model, objModel);
	}

	// std::sort (_drawableObjects.begin (), _drawableObjects.end (), BufferObjectSorter());
}

void Model3DRenderer::Draw ()
{
	Pipeline::SetObjectTransform (_transform);

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		// if (i == 0 || _drawableObjects [i].MAT_NAME != _drawableObjects [i-1].MAT_NAME) {
			Material* mat = MaterialManager::Instance ()->GetMaterial (_drawableObjects [i].MAT_NAME);

			Pipeline::SendMaterial (mat);
		// }

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}
}

void Model3DRenderer::DrawGeometry ()
{
	Pipeline::SetObjectTransform (_transform);

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Pipeline::UpdateMatrices (nullptr);

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}
}

void Model3DRenderer::Clear ()
{
	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		GL::DeleteBuffers(1, &_drawableObjects[i].VBO_INDEX);
		GL::DeleteBuffers(1, &_drawableObjects[i].VBO_INSTANCE_INDEX);
		GL::DeleteBuffers(1, &_drawableObjects[i].IBO_INDEX);
		GL::DeleteVertexArrays(1, &_drawableObjects [i].VAO_INDEX);
	}

	_drawableObjects.clear ();
	_drawableObjects.shrink_to_fit ();
}

void Model3DRenderer::ProcessObjectModel (const Resource<Model>& model, ObjectModel* objModel)
{
	for (PolygonGroup* polyGroup : *objModel) {
		BufferObject bufObj = ProcessPolygonGroup (model, polyGroup);

		_drawableObjects.push_back (bufObj);
	}
}

// For the moment I clone the vertex/normal/texture tuple for every one
// Maybe will be a good idea to delete the duplicates, low priority TODO:
BufferObject Model3DRenderer::ProcessPolygonGroup (const Resource<Model>& model, PolygonGroup* polyGroup)
{
	std::vector<VertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	std::size_t polygonIndex = 0;

	for (Polygon* polygon : *polyGroup) {
		for(std::size_t j=0;j<polygon->VertexCount();j++) {
			VertexData vertexData;

			glm::vec3 position = model->GetVertex (polygon->GetVertex(j));
			vertexData.position[0] = position.x;
			vertexData.position[1] = position.y;
			vertexData.position[2] = position.z;

			if (polygon->HaveNormals ()) {
				glm::vec3 normal = model->GetNormal (polygon->GetNormal(j));
				vertexData.normal[0] = normal.x;
				vertexData.normal[1] = normal.y;
				vertexData.normal[2] = normal.z;
			}

			if (model->HaveUV()) {
				glm::vec2 texcoord = model->GetTexcoord (polygon->GetTexcoord(j));
				vertexData.texcoord[0] = texcoord.x;
				vertexData.texcoord[1] = texcoord.y;
			}

			vertexBuffer.push_back (vertexData);
		}

		indexBuffer.push_back(3 * (unsigned int) polygonIndex);
		indexBuffer.push_back(3 * (unsigned int) polygonIndex + 1);
		indexBuffer.push_back(3 * (unsigned int) polygonIndex + 2);

		polygonIndex ++;
	}

	BufferObject bufObj = BindVertexData (vertexBuffer, indexBuffer);
	bufObj.MAT_NAME = polyGroup->GetMaterialName ();

	return bufObj;
}

BufferObject Model3DRenderer::BindVertexData (const std::vector<VertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays(1 , &VAO);
	GL::BindVertexArray(VAO);

	//creeaza vbo
	GL::GenBuffers(1, &VBO);
	GL::BindBuffer(GL_ARRAY_BUFFER, VBO);
	GL::BufferData(GL_ARRAY_BUFFER, sizeof(VertexData)*vBuf.size(), vBuf.data(), GL_STATIC_DRAW);
	
	//creeaza ibo
	GL::GenBuffers(1, &IBO);
	GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*iBuf.size(), iBuf.data(), GL_STATIC_DRAW);
	
	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray(0);																	//activare pipe 0
	GL::VertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray(1);																	//activare pipe 1
	GL::VertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float) * 3));		//trimite normale pe pipe 1
	GL::EnableVertexAttribArray(2);																	//activare pipe 2
	GL::VertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float) * 6));		//trimite texcoorduri pe pipe 2

	//numar de indecsi
	std::size_t indexCount = iBuf.size();

	BufferObject bufferObject;
	bufferObject.VAO_INDEX = VAO;
	bufferObject.VBO_INDEX = VBO;
	bufferObject.IBO_INDEX = IBO;
	bufferObject.VBO_INSTANCE_INDEX = 0;
	bufferObject.INDEX_COUNT = indexCount;

	return bufferObject;
}
