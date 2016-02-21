#include "Random.h"

#include <cstdlib>
#include <ctime>

#include "Core/Console/Console.h"

Random::Random ()
{
	srand (time (NULL));
}

Random& Random::Instance ()
{
	static Random random;

	return random;
}

int Random::Next () const
{
	return rand ();
}

int Random::RangeI (int a, int b) const
{
	if (a > b) {
		Console::LogWarning ("Random range is invalid. Values swaped. (" +
			std::to_string (a) + " > " + std::to_string (b) + ")");

		std::swap (a, b);
	}

	int random = a + Next () % (b - a + 1);

	return random;
}

// Source: http://stackoverflow.com/questions/686353/c-random-float-number-generation
float Random::RangeF (float a, float b) const
{
	if (a > b) {
		Console::LogWarning ("Random range is invalid. Values swaped. (" +
			std::to_string (a) + " > " + std::to_string (b) + ")");

		std::swap (a, b);
	}

	float random = a + static_cast <float> (Next ()) / (static_cast <float> (RAND_MAX / (b - a + 0.01)));

	return random;
}

void Random::SetSeed (unsigned int seed)
{
	srand (seed);
}