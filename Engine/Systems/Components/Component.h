#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Interfaces/Object.h"

class Component : public Object
{
public:
	Component ();
	virtual ~Component ();

	virtual void Awake ();
	virtual void Start ();

	virtual void Update ();
	virtual void LateUpdate ();
};

#endif