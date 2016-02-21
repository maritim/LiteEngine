#ifndef DISJOINTSETS_H
#define DISJOINTSETS_H

#include <vector>

class DisjointSets
{
public:
	DisjointSets ();
	DisjointSets (std::size_t count);
	void Union (std::size_t firstElement, std::size_t secondElement);
	std::size_t Un
};

#endif