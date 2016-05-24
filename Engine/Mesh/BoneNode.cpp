#include "BoneNode.h"

#include "Core/Console/Console.h"

BoneNode::BoneNode () :
	_parent (nullptr)
{

}

BoneNode::BoneNode (BoneNode* parent) :
	_parent (parent)
{

}

BoneNode::~BoneNode ()
{
	Clear ();
}

std::string BoneNode::GetName () const
{
	return _name;
}

void BoneNode::SetName (const std::string& name)
{
	_name = name;
}

glm::mat4 BoneNode::GetTransform () const
{
	return _transform;
}

void BoneNode::SetTransform (const glm::mat4& transform)
{
	_transform = transform;
}

BoneNode* BoneNode::GetParent ()
{
	return _parent;
}

void BoneNode::SetParent (BoneNode* parent)
{
	_parent = parent;
}

std::size_t BoneNode::GetChildrenCount () const
{
	return _children.size ();
}

BoneNode* BoneNode::GetChild (std::size_t index)
{
	if (index < 0 || index >= _children.size ()) {
		Console::LogWarning ("Bone children size exceed. You are trying to get " +
			std::to_string (index) + " from " + std::to_string (_children.size ()));

		return nullptr;
	}

	return _children [index];
}

void BoneNode::AddChild (BoneNode* child)
{
	_children.push_back (child);
}

void BoneNode::Clear ()
{
	for (auto child : _children) {
		delete child;
	}

	_children.clear ();
	_children.shrink_to_fit ();
}