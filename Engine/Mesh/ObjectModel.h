#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <string>
#include <vector>

#include "PolygonGroup.h"

class ObjectModel
{
private:
	std::string _name;
	std::vector<PolygonGroup*> _polygonGroups;
public:
	ObjectModel(std::string name);
	ObjectModel (const ObjectModel& other);
	~ObjectModel();

	void AddPolygonGroup(PolygonGroup* polygonGroup);

	PolygonGroup* GetPolygonGroup(std::size_t index) const;
	std::size_t GetPolygonCount() const;

	std::string GetName() const;
	
	void SetName(std::string name);

	void Clear();
};

#endif