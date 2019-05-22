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

bool Intersection::CheckFrustumVsPrimitive (FrustumVolume* frustum, GeometricPrimitive* primitive)
{
	AABBVolume* aabb = dynamic_cast<AABBVolume*> (primitive);

	return CheckFrustumVsAABB (frustum, aabb);
}

bool Intersection::CheckFrustumVsAABB (FrustumVolume* frustum, AABBVolume* aabb)
{
	FrustumVolume::FrustumVolumeInformation* frustumData = frustum->GetVolumeInformation ();
	AABBVolume::AABBVolumeInformation* aabbData = aabb->GetVolumeInformation ();

	for (std::size_t i=0;i<FrustumVolume::FrustumVolumeInformation::PLANESCOUNT;i++) {
		// this is the current plane
		const glm::vec4& plane = frustumData->plane [i];

		// p-vertex selection
		const float px = std::signbit (plane.x) ? aabbData->minVertex.x : aabbData->maxVertex.x;
		const float py = std::signbit (plane.y) ? aabbData->minVertex.y : aabbData->maxVertex.y;
		const float pz = std::signbit (plane.z) ? aabbData->minVertex.z : aabbData->maxVertex.z;

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

bool Intersection::CheckRayVsPrimitive (RayPrimitive* ray, GeometricPrimitive* primitive, float& distance)
{
	AABBVolume* aabb = dynamic_cast<AABBVolume*> (primitive);

	return CheckRayVsAABB (ray, aabb, distance);
}

bool Intersection::CheckRayVsAABB (RayPrimitive* ray, AABBVolume* aabb, float& distance)
{
	const RayPrimitive::RayPrimitiveInformation& rayData = ray->GetVolumeInformation ();
	AABBVolume::AABBVolumeInformation* aabbData = aabb->GetVolumeInformation ();

	float tMin, tMax;

	glm::vec3 invRayDir = glm::vec3 (1.0f) / rayData.direction;
	glm::vec3 t1 = (aabbData->minVertex - rayData.origin) * invRayDir;
	glm::vec3 t2 = (aabbData->maxVertex - rayData.origin) * invRayDir;

	glm::vec3 tmin = glm::min(t1, t2);
	glm::vec3 tmax = glm::max(t1, t2);

	tMin = std::max(tmin.x, std::max(tmin.y, tmin.z));
	tMax = std::min(tmax.x, std::min(tmax.y, tmax.z));

	if (tMax > tMin) {
		distance = tMin;
	}

	return tMax > tMin;
}

bool Intersection::CheckRayVsModel (RayPrimitive* ray, const Resource<Model>& model, float& distance)
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

bool Intersection::CheckRayVsPolygon (RayPrimitive* ray, const Resource<Model>& model, Polygon* poly, float& distance)
{
	const RayPrimitive::RayPrimitiveInformation& rayData = ray->GetVolumeInformation ();

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
