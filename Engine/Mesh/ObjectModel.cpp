#include "ObjectModel.h"

#include <vector>
#include <string>

#include "PolygonGroup.h"

ObjectModel::ObjectModel(std::string name) :
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

ObjectModel::~ObjectModel()
{
	for (std::size_t i=0;i<_polygonGroups.size();i++) {
		delete _polygonGroups[i];
	}
}

void ObjectModel::AddPolygonGroup(PolygonGroup* polygonGroup)
{
	_polygonGroups.push_back(polygonGroup);
}

PolygonGroup* ObjectModel::GetPolygonGroup(std::size_t index) const
{
	if (index >= _polygonGroups.size()) {
		return NULL;
	}
	return _polygonGroups [index];
}

std::size_t ObjectModel::GetPolygonCount() const
{
	return _polygonGroups.size();
}

std::string ObjectModel::GetName() const
{
	return _name;
}

void ObjectModel::SetName(std::string name)
{
	_name = name;
}

void ObjectModel::Clear()
{
	for (std::size_t i=0;i<_polygonGroups.size();i++) {
		delete _polygonGroups[i];
	}
	_polygonGroups.clear();
	_polygonGroups.shrink_to_fit();	
}