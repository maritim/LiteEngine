#ifndef FRUSTUMVOLUME_H
#define FRUSTUMVOLUME_H

#include "GeometricPrimitive.h"

#include <glm/glm.hpp>

class FrustumVolume : public GeometricPrimitive
{
public:
	struct FrustumVolumeInformation
	{
		const static int PLANESCOUNT = 6;

		glm::vec4 plane [PLANESCOUNT];
	};

private:
	FrustumVolumeInformation* _data;

public:
	FrustumVolume (FrustumVolumeInformation* data);
	FrustumVolume (const glm::mat4& mvpMatrix);

	FrustumVolumeInformation* GetVolumeInformation () const;
};

#endif