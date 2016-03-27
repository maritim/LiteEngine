#include "Model3DRenderer.h"

#include <GL/glew.h>
#include <string>
#include <vector>

#include "Core/Math/Vector3.h"

#include "Renderer/Pipeline.h"

#include "Material/Material.h"
#include "Managers/MaterialManager.h"
#include "Mesh/Polygon.h"

#include "Wrappers/OpenGL/GL.h"

Model3DRenderer::~Model3DRenderer ()
{
	Clear ();
}

void Model3DRenderer::Attach (Model* model)
{
	for (std::size_t i=0;i<model->ObjectsCount ();i++) {
		ProcessObjectModel (model, model->GetObject (i));
	}
}

void Model3DRenderer::Draw ()
{
	GL::DepthMask (GL_TRUE);

	Pipeline::SetObjectTransform (_transform);
	
	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Material* mat = MaterialManager::Instance ().GetMaterial (_drawableObjects [i].MAT_NAME);

		if (mat == NULL) {
			mat = MaterialManager::Instance ().Default ();
		}

		GL::BlendFunc (mat->blending.first, mat->blending.second);

		Pipeline::SendMaterial (mat);

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}
}

void Model3DRenderer::Clear ()
{
	for (std::size_t i=0;i<_drawableObjects.size ();i++) {		
		GL::DeleteVertexArrays(1, &_drawableObjects [i].VAO_INDEX);
		GL::DeleteBuffers(1, &_drawableObjects [i].VBO_INDEX);
		GL::DeleteBuffers(1, &_drawableObjects [i].VBO_INSTANCE_INDEX);
		GL::DeleteBuffers(1, &_drawableObjects [i].IBO_INDEX);
	}

	_drawableObjects.clear ();
	_drawableObjects.shrink_to_fit ();
}

void Model3DRenderer::ProcessObjectModel (Model* model, ObjectModel* objModel)
{
	for (std::size_t i=0;i<objModel->GetPolygonCount ();i++) {
		BufferObject bufObj = ProcessPolygonGroup (model, objModel->GetPolygonGroup (i));

		_drawableObjects.push_back (bufObj);
	}
}

// For the moment I clone the vertex/normal/texture tuple for every one
// Maybe will be a good idea to delete the duplicates, low priority TODO:
BufferObject Model3DRenderer::ProcessPolygonGroup (Model* model, PolygonGroup* polyGroup)
{
	std::vector<VertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	for (std::size_t i=0;i<polyGroup->GetPolygonCount ();i++) {
		Polygon* polygon = polyGroup->GetPolygon (i);

		for(std::size_t j=0;j<polygon->VertexCount();j++) {
			VertexData vertexData;

			Vector3* position = model->GetVertex (polygon->GetVertex(j));
			vertexData.position[0] = position->x;
			vertexData.position[1] = position->y;
			vertexData.position[2] = position->z;

			if (polygon->HaveNormals ()) {
				Vector3* normal = model->GetNormal (polygon->GetNormal(j));
				vertexData.normal[0] = normal->x;
				vertexData.normal[1] = normal->y;
				vertexData.normal[2] = normal->z;
			}

			if (model->HaveUV()) {
				Vector3* texcoord = model->GetTexcoord (polygon->GetTexcoord(j));
				vertexData.texcoord[0] = texcoord->x;
				vertexData.texcoord[1] = texcoord->y;
			}

			vertexBuffer.push_back (vertexData);
		}

		indexBuffer.push_back (3 * i);
		indexBuffer.push_back (3 * i + 1);
		indexBuffer.push_back (3 * i + 2);
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
	GL::BufferData(GL_ARRAY_BUFFER, sizeof(VertexData)*vBuf.size(), &vBuf[0], GL_STATIC_DRAW);
	
	//creeaza ibo
	GL::GenBuffers(1, &IBO);
	GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*iBuf.size(), &iBuf[0], GL_STATIC_DRAW);
	
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