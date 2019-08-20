#include "ModelView.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

ModelView::~ModelView ()
{
	for (std::size_t i=0;i<_objectBuffers.size ();i++) {
		GL::DeleteBuffers(1, &_objectBuffers[i].VBO_INDEX);
		GL::DeleteBuffers(1, &_objectBuffers[i].VBO_INSTANCE_INDEX);
		GL::DeleteBuffers(1, &_objectBuffers[i].IBO_INDEX);
		GL::DeleteVertexArrays(1, &_objectBuffers [i].VAO_INDEX);
	}
}

void ModelView::Draw ()
{
	for (std::size_t i=0;i<_objectBuffers.size ();i++) {
		if (_objectBuffers [i].materialView != nullptr) {
			Pipeline::SendMaterial (_objectBuffers [i].materialView);
		}

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_objectBuffers [i].VAO_INDEX);
		//comanda desenare
		if (_objectBuffers [i].VBO_INSTANCE_INDEX == 0) {
			GL::DrawElements (GL_TRIANGLES, _objectBuffers [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
		}

		if (_objectBuffers [i].VBO_INSTANCE_INDEX != 0) {
			GL::DrawElementsInstanced(GL_TRIANGLES, _objectBuffers [i].INDEX_COUNT, GL_UNSIGNED_INT, 0, _objectBuffers [i].INSTANCES_COUNT);
		}
	}
}

void ModelView::AddObjectBuffer (const ObjectBuffer& objectBuffer)
{
	_objectBuffers.push_back (objectBuffer);
}

std::vector<ObjectBuffer>::iterator ModelView::begin ()
{
	return _objectBuffers.begin ();
}

std::vector<ObjectBuffer>::iterator ModelView::end ()
{
	return _objectBuffers.end ();
}
