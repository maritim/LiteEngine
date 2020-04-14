#include "Intersection.h"

#include <cmath>
#include <algorithm>

Intersection::Intersection ()
{

}

Intersection::~Intersection ()
{

}

SPECIALIZE_SINGLETON(Intersection)

bool Intersection::CheckFrustumVsAABB (const FrustumVolume& frustum, const AABBVolume& boundingBox)
{
	for (std::size_t i=0;i<FrustumVolume::PLANESCOUNT;i++) {
		// this is the current plane
		const glm::vec4& plane = frustum.plane [i];

		// p-vertex selection
		const float px = std::signbit (plane.x) ? boundingBox.minVertex.x : boundingBox.maxVertex.x;
		const float py = std::signbit (plane.y) ? boundingBox.minVertex.y : boundingBox.maxVertex.y;
		const float pz = std::signbit (plane.z) ? boundingBox.minVertex.z : boundingBox.maxVertex.z;

		// dot product
		// project p-vertex on plane normal
		// (How far is p-vertex from the origin)
		const float dp = (plane.x * px) + (plane.y * py) + (plane.z * pz);

		// doesn't intersect if it is behind the plane
		if (dp < -plane.w) {
			return false;
		}
	}

	   //  int out;
    // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x > box.mMaxX)?1:0); if( out==8 ) return false;
    // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x < box.mMinX)?1:0); if( out==8 ) return false;
    // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y > box.mMaxY)?1:0); if( out==8 ) return false;
    // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y < box.mMinY)?1:0); if( out==8 ) return false;
    // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z > box.mMaxZ)?1:0); if( out==8 ) return false;
    // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z < box.mMinZ)?1:0); if( out==8 ) return false;

	return true;
}

bool Intersection::CheckRayVsAABB (const RayPrimitive& rayData, const AABBVolume& aabbData, float& distance)
{
	float tMin, tMax;

	glm::vec3 invRayDir = glm::vec3 (1.0f) / rayData.direction;
	glm::vec3 t1 = (aabbData.minVertex - rayData.origin) * invRayDir;
	glm::vec3 t2 = (aabbData.maxVertex - rayData.origin) * invRayDir;

	glm::vec3 tmin = glm::min(t1, t2);
	glm::vec3 tmax = glm::max(t1, t2);

	tMin = std::max(tmin.x, std::max(tmin.y, tmin.z));
	tMax = std::min(tmax.x, std::min(tmax.y, tmax.z));

	if (tMax > tMin) {
		distance = tMin;
	}

	return tMax > tMin;
}

bool Intersection::CheckRayVsModel (const RayPrimitive& ray, const Resource<Model>& model, float& distance)
{
	bool isIntersection = false;
	distance = std::numeric_limits<float>::infinity ();

	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			for (Polygon* poly : *polyGroup) {
				float dist = std::numeric_limits<float>::infinity ();

				if (CheckRayVsPolygon (ray, model, poly, dist)) {
					isIntersection = true;

					if (dist < distance) {
						distance = dist;
					}
				}
			}
		}
	}

	return isIntersection;
}

/*
 * Moller–Trumbore algorithm
 * Thanks to: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
*/

bool Intersection::CheckRayVsPolygon (const RayPrimitive& rayData, const Resource<Model>& model, Polygon* poly, float& distance)
{
	const float EPSILON = 0.0000001;

	glm::vec3 v0 = model->GetVertex (poly->GetVertex (0));
	glm::vec3 v1 = model->GetVertex (poly->GetVertex (1));
	glm::vec3 v2 = model->GetVertex (poly->GetVertex (2));

	glm::vec3 edge1, edge2, h, s, q;
	float a,f,u,v;

	edge1 = v1 - v0;
	edge2 = v2 - v0;

	h = glm::cross (rayData.direction, edge2);
	a = glm::dot (edge1, h);

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.

	f = 1.0/a;
	s = rayData.origin - v0;
	u = f * (glm::dot (s, h));

	if (u < 0.0 || u > 1.0)
		return false;

	q = glm::cross (s, edge1);
	v = f * glm::dot (rayData.direction, q);

	if (v < 0.0 || u + v > 1.0)
		return false;

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot (edge2, q);

	if (t > EPSILON) {  // ray intersection
		distance = t;
	}

	return t > EPSILON;
}
