#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object
{
public:
	virtual std::string ToString();
	virtual bool Equal(Object* other);
};

#endif