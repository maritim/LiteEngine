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
	PolygonGroup(std::string name);
	PolygonGroup (const PolygonGroup& other);
	~PolygonGroup();

	std::string GetName() const;
	std::string GetMaterialName() const;

	void SetName(std::string name);
	void SetMaterialName(std::string matName);

	Polygon* GetPolygon(std::size_t index) const;
	std::size_t GetPolygonCount() const;

	void AddPolygon(Polygon* polygon);
	void Clear();
};

#endif