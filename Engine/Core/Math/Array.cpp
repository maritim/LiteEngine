#include <iostream>
#include "Array.h"

Array::Array(int size) :
	_array (nullptr),
	_length(size)
{
	_array =new long double[_length];

	for (int i=0;i<_length;i++) {
		_array [i] = 0.0;
	}
}

Array::Array (const Array& other) :
	_array (nullptr),
	_length (other._length)
{
	_array = new long double[_length];

	for (int i=0;i<_length;i++) {
		_array [i] = other._array [i];
	}
}

Array& Array::operator=(const Array& other)
{
	delete[] _array;

	_length = other._length;

	_array = new long double[_length];

	for (int i=0;i<_length;i++) {
		_array [i] = other._array [i];
	}

	return *this;
}

long double& Array::operator [](const int k)
{
	if (k < 0 || k >= _length) {
		throw "Error. Array limits exceeded...";
	}

	return _array[k];
}

Array::~Array (void) 
{
	delete[] _array;
}
