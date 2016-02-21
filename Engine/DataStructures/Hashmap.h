#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>

#define MAGIC_NUMBER 666013

class Hashmap
{
public:
	Hashmap(void);
	~Hashmap(void);
	void Add(int head, int tail, int middle);
	int Get(int head, int tail);

private:
	std::vector< std::vector<std::pair<std::pair<int, int>, int> > > _map;
};

#endif