#include "AABBVolume.h"

AABBVolume::AABBVolume (AABBVolumeInformation* data) :
	GeometricPrimitive (),
	_data (data)
{

}

AABBVolume::AABBVolumeInformation* AABBVolume::GetVolumeInformation () const
{
	return _data;
}