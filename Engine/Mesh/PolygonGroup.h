#ifndef POLYGONGROUP_H
#define POLYGONGROUP_H

#include "Polygon.h"

#include <string>
#include <vector>

class PolygonGroup
{
private:
	std::string _name;
	std::string _matName;
	std::vector<Polygon*> _polygons;
public:
	PolygonGroup (std::string name);
	PolygonGroup (const PolygonGroup& other);
	~PolygonGroup ();

	std::string GetName () const;
	std::string GetMaterialName () const;

	void SetName (const std::string& name);
	void SetMaterialName (const std::string& matName);

	std::vector<Polygon*>::iterator begin ();
	std::vector<Polygon*>::iterator end ();

	void AddPolygon (Polygon* polygon);
	void Clear ();
};

#endif