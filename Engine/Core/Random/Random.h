#ifndef RANDOM_H
#define RANDOM_H

// TODO: Improve this

class Random
{
public:
	static Random& Instance ();

	int Next () const;
	int RangeI (int a, int b) const;
	float RangeF (float a, float b) const;

	void SetSeed (unsigned int seed);
private:
	Random ();
};

#endif