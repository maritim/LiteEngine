#include "Polygon.h"

Polygon::Polygon(void)
{

}

Polygon::Polygon(const Polygon& other) :
	_vertices (other._vertices),
	_normals (other._normals),
	_texcoords (other._texcoords)
{

}

void Polygon::AddVertex(int position)
{
	_vertices.push_back(position);
}

void Polygon::AddVertex(int vertPos, int texcoordPos, int normPos)
{
	_vertices.push_back(vertPos);
	_normals.push_back(normPos);

	_texcoords.push_back(texcoordPos);
}

void Polygon::AddTexcoord(int position)
{
	_texcoords.push_back (position);
}

void Polygon::AddNormal(int position)
{
	_normals.push_back (position);
}

int Polygon::GetVertex(std::size_t position) const
{
	return _vertices[position];
}

int Polygon::GetTexcoord(std::size_t position) const
{
	if (position >= _texcoords.size ()) {
		return 0;
	}

	return _texcoords[position];
}

int Polygon::GetNormal(std::size_t position) const
{
	return _normals[position];
}

void Polygon::SetVertex(int modelPos, std::size_t polygonPos)
{
	_vertices [polygonPos] = modelPos;
}

void Polygon::SetTexcoord (int modelPos, std::size_t polygonPos)
{
	_texcoords [polygonPos] = modelPos;
}

void Polygon::SetNormal (int modelPos, std::size_t polygonPos)
{
	_normals [polygonPos] = modelPos;
}

std::size_t Polygon::VertexCount(void) const
{
	return _vertices.size ();
}

void Polygon::ReverseVertexOrder(void)
{
	for (int i=0;i<(int)_vertices.size() >> 1;i++) {
		std::swap (_vertices [i], _vertices [_vertices.size() - 1 - i]);

		if (_normals.size () > 0) {
			std::swap (_normals [i], _normals [_vertices.size() - 1 - i]);
		}

		if (_texcoords.size() > 0) {
			std::swap (_texcoords [i], _texcoords [_vertices.size() - 1 - i]);
		}
	}
}

void Polygon::ClearTexcoords(void)
{
	_texcoords.clear ();
}

bool Polygon::HaveNormals (void) const
{
	return _normals.size () > 0;
}

bool Polygon::HaveUV () const
{
	return _texcoords.size () > 0;
}

Polygon::~Polygon()
{

}
