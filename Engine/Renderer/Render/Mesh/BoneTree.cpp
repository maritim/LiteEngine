#include "BoneTree.h"

BoneTree::BoneTree () :
	_root (nullptr)
{

}

BoneTree::~BoneTree ()
{
	Clear ();
}

BoneNode* BoneTree::GetRoot ()
{
	return _root;
}

void BoneTree::SetRoot (BoneNode* root)
{
	Clear ();

	_root = root;
}

void BoneTree::Clear ()
{
	if (_root == nullptr) {
		return ;
	}

	delete _root;	
}