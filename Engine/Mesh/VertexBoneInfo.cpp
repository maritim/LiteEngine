#include "VertexBoneInfo.h"

#include "Core/Console/Console.h"

std::size_t VertexBoneInfo::GetBoneIDsCount () const
{
	return _boneIDs.size ();
}

std::size_t VertexBoneInfo::GetBoneWeightsCount () const
{
	return _boneWeights.size ();
}

int VertexBoneInfo::GetBoneID (std::size_t index) const
{
	if (index < 0 || index >= _boneIDs.size ()) {
		Console::LogError ("Bone IDs size exceeded. You are trying to get" + std::to_string (index) + 
			" but there are " + std::to_string (_boneIDs.size ()) + " bones!");

		return 0;
	}

	return _boneIDs [index];
}

float VertexBoneInfo::GetBoneWeight (std::size_t index) const
{
	if (index < 0 || index >= _boneWeights.size ()) {
		Console::LogError ("Bone weights size exceeded. You are trying to get" + std::to_string (index) + 
			" but there are " + std::to_string (_boneWeights.size ()) + " bones!");

		return 0.0f;
	}

	return _boneWeights [index];	
}

void VertexBoneInfo::AddBone (int id, float weight)
{
	_boneIDs.push_back (id);
	_boneWeights.push_back (weight);
}