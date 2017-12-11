#ifndef RANDOM_H
#define RANDOM_H

// TODO: Improve this

#include "Core/Singleton/Singleton.h"

class Random : public Singleton<Random>
{
	friend Singleton<Random>;

public:
	int Next () const;
	int RangeI (int a, int b) const;
	float RangeF (float a, float b) const;

	void SetSeed (unsigned int seed);
private:
	Random ();
	~Random ();
	Random (const Random& other);
	Random& operator= (const Random&);
};

#endif