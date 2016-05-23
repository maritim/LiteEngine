#ifndef BONENODE_H
#define BONENODE_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <string>

#include <assimp/scene.h>

class BoneNode : public Object
{
protected:
	std::string _name;
	aiMatrix4x4 _transform;
	BoneNode* _parent;
	std::vector<BoneNode*> _children;

public:
	BoneNode ();
	BoneNode (BoneNode* parent);
	~BoneNode ();

	std::string GetName () const;
	void SetName (const std::string& name);

	aiMatrix4x4 GetTransform () const;
	void SetTransform (aiMatrix4x4 transform);

	BoneNode* GetParent ();
	void SetParent (BoneNode* parent);

	std::size_t GetChildrenCount () const;
	BoneNode* GetChild (std::size_t index);
	void AddChild (BoneNode* child);
protected:
	void Clear ();
};

#endif