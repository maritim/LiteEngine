#ifndef ANIMATIONNODE_H
#define ANIMATIONNODE_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <assimp/scene.h>

class AnimationNode : public Object
{
protected:
	std::vector<aiQuatKey> _rotationKeys;
	std::vector<aiVectorKey> _scalingKeys;
	std::vector<aiVectorKey> _positionKeys;

public:
	std::size_t GetRotationKeysCount () const;
	std::size_t GetScalingKeysCount () const;
	std::size_t GetPositionKeysCount () const;

	aiQuatKey GetRotationKey (std::size_t index);
	aiVectorKey GetScalingKey (std::size_t index);
	aiVectorKey GetPositionKey (std::size_t index);

	void AddRotationKey (aiQuatKey rotKey);
	void AddScalingKey (aiVectorKey scaleKey);
	void AddPositionKey (aiVectorKey positionKey);
};

#endif