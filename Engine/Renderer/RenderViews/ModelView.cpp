#include "ModelView.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

ModelView::~ModelView ()
{
	GL::DeleteBuffers(1, &_objectBuffer.VBO_INDEX);
	GL::DeleteBuffers(1, &_objectBuffer.VBO_INSTANCE_INDEX);
	GL::DeleteBuffers(1, &_objectBuffer.IBO_INDEX);
	GL::DeleteVertexArrays(1, &_objectBuffer.VAO_INDEX);
}

void ModelView::Draw ()
{
	//bind pe containerul de stare de geometrie (vertex array object)
	GL::BindVertexArray(_objectBuffer.VAO_INDEX);

	for (std::size_t i=0;i<_groupBuffers.size ();i++) {
		Pipeline::SendMaterial (_groupBuffers [i].materialView);

		//comanda desenare
		if (_objectBuffer.VBO_INSTANCE_INDEX == 0) {
			GL::DrawElements (GL_TRIANGLES, _groupBuffers [i].INDEX_COUNT, GL_UNSIGNED_INT,
				(void*) (sizeof (unsigned int) * _groupBuffers [i].offset));
		}

		if (_objectBuffer.VBO_INSTANCE_INDEX != 0) {
			GL::DrawElementsInstanced(GL_TRIANGLES, _groupBuffers [i].INDEX_COUNT, GL_UNSIGNED_INT,
				(void*) (sizeof (unsigned int) * _groupBuffers [i].offset), _objectBuffer.INSTANCES_COUNT);
		}
	}
}

void ModelView::DrawGeometry ()
{
	Pipeline::UpdateMatrices (nullptr);
	//bind pe containerul de stare de geometrie (vertex array object)
	GL::BindVertexArray(_objectBuffer.VAO_INDEX);

	for (std::size_t i=0;i<_groupBuffers.size ();i++) {
		//comanda desenare
		if (_objectBuffer.VBO_INSTANCE_INDEX == 0) {
			GL::DrawElements (GL_TRIANGLES, _groupBuffers [i].INDEX_COUNT, GL_UNSIGNED_INT,
				(void*) (sizeof (unsigned int) * _groupBuffers [i].offset));
		}

		if (_objectBuffer.VBO_INSTANCE_INDEX != 0) {
			GL::DrawElementsInstanced(GL_TRIANGLES, _groupBuffers [i].INDEX_COUNT, GL_UNSIGNED_INT,
				(void*) (sizeof (unsigned int) * _groupBuffers [i].offset), _objectBuffer.INSTANCES_COUNT);
		}
	}
}

void ModelView::SetObjectBuffer (const ObjectBuffer& objectBuffer)
{
	_objectBuffer = objectBuffer;
}

void ModelView::AddGroupBuffer (const GroupBuffer& groupBuffer)
{
	_groupBuffers.push_back (groupBuffer);
}

ObjectBuffer& ModelView::GetObjectBuffer ()
{
	return _objectBuffer;
}

std::size_t ModelView::GetVerticesCount () const
{
	return _objectBuffer.VerticesCount;
}

std::size_t ModelView::GetPolygonsCount () const
{
	return _objectBuffer.PolygonsCount;
}

std::vector<GroupBuffer>::iterator ModelView::begin ()
{
	return _groupBuffers.begin ();
}

std::vector<GroupBuffer>::iterator ModelView::end ()
{
	return _groupBuffers.end ();
}
