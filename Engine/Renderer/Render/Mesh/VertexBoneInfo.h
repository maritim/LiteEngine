#ifndef VERTEXBONEINFO
#define VERTEXBONEINFO

#include "Core/Interfaces/Object.h"

#include <vector>

class VertexBoneInfo : public Object
{
protected:
	std::vector<int> _boneIDs;
	std::vector<float> _boneWeights;

public:
	std::size_t GetBoneIDsCount () const;
	std::size_t GetBoneWeightsCount () const;

	int GetBoneID (std::size_t index) const;
	float GetBoneWeight (std::size_t index) const;

	void AddBone (int boneID, float boneWeight);
};

#endif