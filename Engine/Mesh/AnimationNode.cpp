#include "AnimationNode.h"

#include "Core/Console/Console.h"

std::size_t AnimationNode::GetRotationKeysCount () const
{
	return _rotationKeys.size ();
}

std::size_t AnimationNode::GetScalingKeysCount () const
{
	return _scalingKeys.size ();
}

std::size_t AnimationNode::GetPositionKeysCount () const
{
	return _positionKeys.size ();
}

aiQuatKey AnimationNode::GetRotationKey (std::size_t index)
{
	if (index < 0 || index >= _rotationKeys.size ()) {
		Console::LogWarning ("Animation rotation key index exceed rotation keys count. \
			You are searching for " + std::to_string (index) + " and the size is " + 
			std::to_string (_rotationKeys.size ()));

		return aiQuatKey ();
	}

	return _rotationKeys [index];
}

aiVectorKey AnimationNode::GetScalingKey (std::size_t index)
{
	if (index < 0 || index >= _scalingKeys.size ()) {
		Console::LogWarning ("Animation scaling key index exceed rotation keys count. \
			You are searching for " + std::to_string (index) + " and the size is " + 
			std::to_string (_scalingKeys.size ()));

		return aiVectorKey ();
	}

	return _scalingKeys [index];
}

aiVectorKey AnimationNode::GetPositionKey (std::size_t index)
{
	if (index < 0 || index >= _positionKeys.size ()) {
		Console::LogWarning ("Animation position key index exceed rotation keys count. \
			You are searching for " + std::to_string (index) + " and the size is " + 
			std::to_string (_positionKeys.size ()));

		return aiVectorKey ();
	}

	return _positionKeys [index];	
}

void AnimationNode::AddRotationKey (aiQuatKey rotKey)
{
	_rotationKeys.push_back (rotKey);
}

void AnimationNode::AddScalingKey (aiVectorKey scaleKey)
{
	_scalingKeys.push_back (scaleKey);
}

void AnimationNode::AddPositionKey (aiVectorKey positionKey)
{
	_positionKeys.push_back (positionKey);
}
