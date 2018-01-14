#include "ObjectModel.h"

ObjectModel::ObjectModel (const std::string& name) :
	_name (name)
{

}

ObjectModel::ObjectModel (const ObjectModel& other) :
	_name (other._name)
{
	for (PolygonGroup* polyGroup : other._polygonGroups) {
		_polygonGroups.push_back (new PolygonGroup (*polyGroup));
	}
}

ObjectModel::~ObjectModel ()
{
	Clear ();
}

void ObjectModel::AddPolygonGroup (PolygonGroup* polygonGroup)
{
	_polygonGroups.push_back (polygonGroup);
}

std::string ObjectModel::GetName () const
{
	return _name;
}

void ObjectModel::SetName (const std::string& name)
{
	_name = name;
}

std::vector<PolygonGroup*>::iterator ObjectModel::begin ()
{
	return _polygonGroups.begin ();
}

std::vector<PolygonGroup*>::iterator ObjectModel::end ()
{
	return _polygonGroups.end ();
}

void ObjectModel::Clear()
{
	for (std::size_t i=0;i<_polygonGroups.size ();i++) {
		delete _polygonGroups [i];
	}
	_polygonGroups.clear ();
	_polygonGroups.shrink_to_fit ();
}