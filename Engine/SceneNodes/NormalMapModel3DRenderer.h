#ifndef NORMALMAPMODEL3DRENDERER_H
#define NORMALMAPMODEL3DRENDERER_H

#include "Model3DRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct NormalMapVertexData : VertexData
{
	float tangent [3];

	NormalMapVertexData ();
};

class ENGINE_API NormalMapModel3DRenderer : public Model3DRenderer
{
public:
	using Model3DRenderer::Model3DRenderer;

	void Draw ();

protected:
	BufferObject ProcessPolygonGroup (const Resource<Model>& model, PolygonGroup* polyGroup);

	BufferObject BindVertexData (const std::vector<NormalMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf);

	glm::vec3 CalculateNormal (const Resource<Model>& model, Polygon* poly);
	glm::vec3 CalculateTangent (const Resource<Model>& model, Polygon* poly);
};

#endif