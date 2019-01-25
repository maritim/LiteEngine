#ifndef OBSERVER_H
#define OBSERVER_H

#include "Core/Interfaces/Object.h"

template <class T>
class ObserverI : public Object
{
public:
	virtual void Notify (Object* sender, const T& args) = 0;
};

#endif