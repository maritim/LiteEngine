#include "Intersection.h"

#include <cmath>

Intersection::Intersection ()
{

}

Intersection::~Intersection ()
{

}

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
		const int px = std::signbit (plane.x) ? (int) aabbData->minVertex.x : (int) aabbData->maxVertex.x;
		const int py = std::signbit (plane.y) ? (int) aabbData->minVertex.y : (int) aabbData->maxVertex.y;
		const int pz = std::signbit (plane.z) ? (int) aabbData->minVertex.z : (int) aabbData->maxVertex.z;

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