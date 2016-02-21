#ifndef ARRAY_H
#define ARRAY_H

//#include "Memory.h"

class Array 
{
//	TRACK_MEMORY(MEMTYPE_MATH_ARRAY)

private: 
     long double* _array;
     int _length;

public:
     Array(int size = 0);
     Array (const Array& other);
     ~Array ();

     Array& operator=(const Array& other);

     long double& operator [](const int k);
};

#endif