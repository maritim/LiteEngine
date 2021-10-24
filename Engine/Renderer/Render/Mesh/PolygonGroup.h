#ifndef POLYGONGROUP_H
#define POLYGONGROUP_H

#include "Polygon.h"

#include <string>
#include <vector>

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Material/Material.h"

class PolygonGroup
{
private:
	std::string _name;
	Resource<Material> _material;
	std::vector<Polygon*> _polygons;
public:
	PolygonGroup (std::string name);
	PolygonGroup (const PolygonGroup& other);
	~PolygonGroup ();

	std::string GetName () const;
	Resource<Material> GetMaterial () const;

	void SetName (const std::string& name);
	void SetMaterial (const Resource<Material>& material);

	std::vector<Polygon*>::iterator begin ();
	std::vector<Polygon*>::iterator end ();

	void AddPolygon (Polygon* polygon);
	void Clear ();
};

#endif