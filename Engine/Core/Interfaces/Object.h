#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object
{
public:
	virtual ~Object ();

	virtual std::string ToString();
	virtual bool Equal(Object* other);
};

#endif