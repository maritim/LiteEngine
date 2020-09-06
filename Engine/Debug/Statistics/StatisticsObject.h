#ifndef STATISTICSOBJECT_H
#define STATISTICSOBJECT_H

#include "Core/Interfaces/Object.h"

#define DECLARE_STATISTICS_OBJECT(T) \
public: \
	static std::string GetName () { return #T; }

class ENGINE_API StatisticsObject : public Object
{

};

#endif