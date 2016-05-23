#ifndef BONETREE_H
#define BONETREE_H

#include "Core/Interfaces/Object.h"

#include "BoneNode.h"

class BoneTree : public Object
{
protected:
	BoneNode* _root;

public:
	BoneTree ();
	~BoneTree ();

	BoneNode* GetRoot ();
	void SetRoot (BoneNode*);
protected:
	void Clear ();
};

#endif