/*************************************************************************** 
 Polygon
 ***************************************************************************/

#ifndef POLYGON_H
#define POLYGON_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <vector>

#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

class Polygon : public Object
{
private:
    std::vector<int> _vertices;
    std::vector<int> _normals;
    std::vector<int> _texcoords;

public:
	Polygon ();
	Polygon (const Polygon& other);

	void AddVertex(int position);
	void AddVertex (int vertexPos, int texcoordPos, int normalPos);
	void AddTexcoord(int position);
	void AddNormal(int position);

	int GetVertex(std::size_t position) const;
	int GetTexcoord(std::size_t position) const;
	int GetNormal(std::size_t position) const;

	void SetVertex(int modelPos, std::size_t polygonPos);
	void SetTexcoord (int modelPos, std::size_t polygonPos);
	void SetNormal (int modelPos, std::size_t polygonPos);

	std::size_t VertexCount(void) const;

	void ReverseVertexOrder(void);
	void ClearTexcoords(void);
	bool HaveNormals (void) const;
	bool HaveUV () const;
    ~Polygon();
};

#endif