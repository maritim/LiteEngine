#include "PolygonGroup.h"

PolygonGroup::PolygonGroup(std::string name) :
	_name (name),
	_material (nullptr)
{

}

PolygonGroup::PolygonGroup (const PolygonGroup& other) :
	_name (other._name),
	_material (other._material)
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

Resource<Material> PolygonGroup::GetMaterial () const
{
	return _material;
}

void PolygonGroup::SetName (const std::string& name)
{
	_name = name;
}

void PolygonGroup::SetMaterial (const Resource<Material>& material)
{
	_material = material;
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