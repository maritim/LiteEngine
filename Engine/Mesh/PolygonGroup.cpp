#include "PolygonGroup.h"

PolygonGroup::PolygonGroup(std::string name) :
	_name (name),
	_matName ()
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

std::string PolygonGroup::GetName () const
{
	return _name;
}

std::string PolygonGroup::GetMaterialName () const
{
	return _matName;
}

void PolygonGroup::SetName (const std::string& name)
{
	_name = name;
}

void PolygonGroup::SetMaterialName (const std::string& materialName)
{
	_matName = materialName;
}

std::vector<Polygon*>::iterator PolygonGroup::begin ()
{
	return _polygons.begin ();
}

std::vector<Polygon*>::iterator PolygonGroup::end ()
{
	return _polygons.end ();
}

void PolygonGroup::Clear ()
{
	for (std::size_t i=0;i<_polygons.size ();i++) {
		delete _polygons [i];
	}
	_polygons.clear ();
	_polygons.shrink_to_fit ();
}

PolygonGroup::~PolygonGroup ()
{
	Clear ();
}