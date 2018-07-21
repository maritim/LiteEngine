#ifndef LIGHTMAPMODEL3DRENDERER_H
#define LIGHTMAPMODEL3DRENDERER_H

#include "Model3DRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct LightMapVertexData : VertexData
{
	float lmTexcoord [2];

	LightMapVertexData ();
};

class LightMapModel3DRenderer : public Model3DRenderer
{
public:
	using Model3DRenderer::Model3DRenderer;

protected:
	BufferObject ProcessPolygonGroup (Model* model, PolygonGroup* polyGroup);

	BufferObject BindVertexData (const std::vector<LightMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf);
};

#endif