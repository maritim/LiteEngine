#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <vector>

#include "PolygonGroup.h"

class ObjectModel : public Object
{
private:
	std::string _name;
	std::vector<PolygonGroup*> _polygonGroups;

public:
	ObjectModel (const std::string& name);
	ObjectModel (const ObjectModel& other);
	~ObjectModel ();

	void AddPolygonGroup (PolygonGroup* polygonGroup);

	std::string GetName () const;
	
	void SetName (const std::string& name);

	std::vector<PolygonGroup*>::iterator begin ();
	std::vector<PolygonGroup*>::iterator end ();

	void Clear ();
};

#endif