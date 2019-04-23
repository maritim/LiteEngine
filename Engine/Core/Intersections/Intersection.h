#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Core/Singleton/Singleton.h"

#include "GeometricPrimitive.h"

#include "FrustumVolume.h"
#include "AABBVolume.h"
#include "RayPrimitive.h"

#include "Mesh/Model.h"

class Intersection : public Singleton<Intersection>
{
	friend class Singleton<Intersection>;

public:
	bool CheckFrustumVsPrimitive (FrustumVolume* frustum, GeometricPrimitive* primitive);
	bool CheckFrustumVsAABB(FrustumVolume* frustum, AABBVolume* aabb);
	bool CheckRayVsPrimitive (RayPrimitive* ray, GeometricPrimitive* aabb, float& distance);
	bool CheckRayVsAABB (RayPrimitive* ray, AABBVolume* aabb, float& distance);
	bool CheckRayVsModel (RayPrimitive* ray, Model* model, float& distance);
	bool CheckRayVsPolygon (RayPrimitive* ray, Model* model, Polygon* poly, float& distance);
private:
	Intersection ();
	Intersection (const Intersection&);
	Intersection& operator=(const Intersection&);
	~Intersection ();
};

#endif