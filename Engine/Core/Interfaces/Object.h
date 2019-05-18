#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class ENGINE_API Object
{
public:
	virtual ~Object ();

	virtual std::string ToString();
	virtual bool Equal(Object* other);
};

#endif