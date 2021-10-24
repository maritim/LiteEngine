#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <vector>

#include "Renderer/Render/Mesh/Model.h"
#include "Renderer/Render/Mesh/Polygon.h"
#include "Renderer/Render/Mesh/PolygonGroup.h"

class Triangulation
{
public:
	static void ConvexTriangulation (Model* model);
	static std::vector<Polygon*> ConvexTriangulation (Polygon* polygon);
private:
	static PolygonGroup* ConvexTriangulation (PolygonGroup* polyGroup);
};

#endif