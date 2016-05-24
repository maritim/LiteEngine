#ifndef BONENODE_H
#define BONENODE_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <string>

#include "Core/Math/glm/glm.hpp"

class BoneNode : public Object
{
protected:
	std::string _name;
	glm::mat4 _transform;
	BoneNode* _parent;
	std::vector<BoneNode*> _children;

public:
	BoneNode ();
	BoneNode (BoneNode* parent);
	~BoneNode ();

	std::string GetName () const;
	void SetName (const std::string& name);

	glm::mat4 GetTransform () const;
	void SetTransform (const glm::mat4& transform);

	BoneNode* GetParent ();
	void SetParent (BoneNode* parent);

	std::size_t GetChildrenCount () const;
	BoneNode* GetChild (std::size_t index);
	void AddChild (BoneNode* child);
protected:
	void Clear ();
};

#endif