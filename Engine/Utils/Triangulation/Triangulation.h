#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <vector>

#include "Mesh/Model.h"
#include "Mesh/Polygon.h"
#include "Mesh/PolygonGroup.h"

class Triangulation
{
public:
	static void ConvexTriangulation (Model* model);
	static std::vector<Polygon*> ConvexTriangulation (Polygon* polygon);
private:
	static PolygonGroup* ConvexTriangulation (PolygonGroup* polyGroup);
};

#endif