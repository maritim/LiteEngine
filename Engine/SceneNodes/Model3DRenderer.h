#ifndef MODEL3DRENDERER_H
#define MODEL3DRENDERER_H

#include "Renderer/Renderer.h"

#include <string>
#include <vector>

#include "Mesh/Model.h"
#include "Mesh/ObjectModel.h"
#include "Mesh/PolygonGroup.h"

struct BufferObject
{
	unsigned int VAO_INDEX;
	unsigned int VBO_INDEX;
	unsigned int VBO_INSTANCE_INDEX;
	unsigned int IBO_INDEX;
	std::string MAT_NAME;
	std::size_t INDEX_COUNT;
};

struct VertexData
{
	float position[3];
	float normal[3];
	float texcoord[2];

	VertexData ();
};

class Model3DRenderer : public Renderer
{
protected:
	std::vector<BufferObject> _drawableObjects;

public:
	using Renderer::Renderer;

	virtual ~Model3DRenderer ();

	virtual void Attach (Model* mesh);

	virtual void Draw ();

	void Clear ();
protected:
	void ProcessObjectModel (Model* model, ObjectModel* objModel);
	virtual BufferObject ProcessPolygonGroup (Model* model, PolygonGroup* polyGroup);

	virtual BufferObject BindVertexData (const std::vector<VertexData>& vBuf, const std::vector<unsigned int>& iBuf);

	void ClearCurrentData ();
};

#endif