#ifndef ANIMATIONNODE_H
#define ANIMATIONNODE_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "Core/Math/glm/glm.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

struct QuatKey
{
	glm::quat value;
	float time;
};

struct VectorKey
{
	glm::vec3 value;
	float time;
};

class AnimationNode : public Object
{
protected:
	std::vector<QuatKey> _rotationKeys;
	std::vector<VectorKey> _scalingKeys;
	std::vector<VectorKey> _positionKeys;

public:
	std::size_t GetRotationKeysCount () const;
	std::size_t GetScalingKeysCount () const;
	std::size_t GetPositionKeysCount () const;

	QuatKey GetRotationKey (std::size_t index);
	VectorKey GetScalingKey (std::size_t index);
	VectorKey GetPositionKey (std::size_t index);

	void AddRotationKey (QuatKey rotKey);
	void AddScalingKey (VectorKey scaleKey);
	void AddPositionKey (VectorKey positionKey);
};

#endif