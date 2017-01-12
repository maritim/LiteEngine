#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Core/Singleton/Singleton.h"

#include "GeometricPrimitive.h"

#include "FrustumVolume.h"
#include "AABBVolume.h"

class Intersection : public Singleton<Intersection>
{
	friend class Singleton<Intersection>;

public:
	bool CheckFrustumVsPrimitive (FrustumVolume* frustum, GeometricPrimitive* primitive);
	bool CheckFrustumVsAABB(FrustumVolume* frustum, AABBVolume* aabb);
private:
	Intersection ();
	Intersection (const Intersection&);
	Intersection& operator=(const Intersection&);
	~Intersection ();
};

#endif