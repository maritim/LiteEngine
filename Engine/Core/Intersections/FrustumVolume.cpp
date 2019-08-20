#include "FrustumVolume.h"

FrustumVolume::FrustumVolume (FrustumVolumeInformation* data) :
	GeometricPrimitive (),
	_data (data)
{

}

FrustumVolume::~FrustumVolume ()
{
	delete _data;
}

void FrustumVolume::SetVolume (const glm::mat4& mvpMatrix)
{
	FrustumVolumeInformation* frustum = (FrustumVolumeInformation*) _data;

	glm::mat4 MVP = glm::transpose (mvpMatrix);

	frustum->plane [0].x = MVP [0][0] + MVP [3][0];
	frustum->plane [0].y = MVP [0][1] + MVP [3][1];
	frustum->plane [0].z = MVP [0][2] + MVP [3][2];
	frustum->plane [0].w = MVP [0][3] + MVP [3][3];

	frustum->plane [1].x = - MVP [0][0] + MVP [3][0];
	frustum->plane [1].y = - MVP [0][1] + MVP [3][1];
	frustum->plane [1].z = - MVP [0][2] + MVP [3][2];
	frustum->plane [1].w = - MVP [0][3] + MVP [3][3];

	frustum->plane [2].x = MVP [1][0] + MVP [3][0];
	frustum->plane [2].y = MVP [1][1] + MVP [3][1];
	frustum->plane [2].z = MVP [1][2] + MVP [3][2];
	frustum->plane [2].w = MVP [1][3] + MVP [3][3];

	frustum->plane [3].x = - MVP [1][0] + MVP [3][0];
	frustum->plane [3].y = - MVP [1][1] + MVP [3][1];
	frustum->plane [3].z = - MVP [1][2] + MVP [3][2];
	frustum->plane [3].w = - MVP [1][3] + MVP [3][3];

	frustum->plane [4].x = MVP [2][0] + MVP [3][0];
	frustum->plane [4].y = MVP [2][1] + MVP [3][1];
	frustum->plane [4].z = MVP [2][2] + MVP [3][2];
	frustum->plane [4].w = MVP [2][3] + MVP [3][3];

	frustum->plane [5].x = - MVP [2][0] + MVP [3][0];
	frustum->plane [5].y = - MVP [2][1] + MVP [3][1];
	frustum->plane [5].z = - MVP [2][2] + MVP [3][2];
	frustum->plane [5].w = - MVP [2][3] + MVP [3][3];

	for (std::size_t i=0;i<6;i++) {
		frustum->plane [i] = glm::normalize (frustum->plane [i]);
	}
}

FrustumVolume::FrustumVolumeInformation* FrustumVolume::GetVolumeInformation () const
{
	return _data;
}
