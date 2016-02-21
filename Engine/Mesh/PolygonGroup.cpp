#include "PolygonGroup.h"

#include <string>
#include <vector>

PolygonGroup::PolygonGroup(std::string name) :
	_name (name),
	_matName ()	// Maybe change here with global default material
{

}

PolygonGroup::PolygonGroup (const PolygonGroup& other) :
	_name (other._name),
	_matName (other._matName)
{
	for (Polygon* polygon : other._polygons) {
		_polygons.push_back (new Polygon (*polygon));
	}
}

void PolygonGroup::AddPolygon (Polygon* polygon)
{
	_polygons.push_back (polygon);
}

std::string PolygonGroup::GetName() const
{
	return _name;
}

void PolygonGroup::SetName(std::string name)
{
	_name = name;
}

std::string PolygonGroup::GetMaterialName() const
{
	return _matName;
}

void PolygonGroup::SetMaterialName(std::string materialName)
{
	_matName = materialName;
}

Polygon* PolygonGroup::GetPolygon(std::size_t index) const
{
	if (index >= _polygons.size()) {
		return NULL;
	}
	return _polygons [index];
}

std::size_t PolygonGroup::GetPolygonCount() const
{
	return _polygons.size();
}

void PolygonGroup::Clear ()
{
	for (std::size_t i=0;i<_polygons.size();i++) {
		delete _polygons[i];
	}
	_polygons.clear ();
	_polygons.shrink_to_fit ();
}

PolygonGroup::~PolygonGroup()
{
	Clear ();
}