#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Core/Singleton/Singleton.h"

#include "FrustumVolume.h"
#include "AABBVolume.h"
#include "RayPrimitive.h"

#include "Core/Resources/Resource.h"
#include "Mesh/Model.h"

class ENGINE_API Intersection : public Singleton<Intersection>
{
	friend class Singleton<Intersection>;

	DECLARE_SINGLETON(Intersection)

public:
	bool CheckFrustumVsAABB (const FrustumVolume&, const AABBVolume&);
	bool CheckRayVsAABB (const RayPrimitive& ray, const AABBVolume& aabb, float& distance);
	bool CheckRayVsModel (const RayPrimitive& ray, const Resource<Model>& model, float& distance);
	bool CheckRayVsPolygon (const RayPrimitive& ray, const Resource<Model>& model, Polygon* poly, float& distance);
private:
	Intersection ();
	Intersection (const Intersection&);
	Intersection& operator=(const Intersection&);
	~Intersection ();
};

#endif