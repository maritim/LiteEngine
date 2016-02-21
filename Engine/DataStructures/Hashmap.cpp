#include <vector>
#include <cmath>
#include <algorithm>

#include "Hashmap.h"

Hashmap::Hashmap(void) :
	_map ()
{
	for (int i=0;i<MAGIC_NUMBER;i++) {
		_map.push_back (std::vector<std::pair<std::pair<int, int>, int> >());
	}
}

Hashmap::~Hashmap(void)
{
	for (int i=0;i<MAGIC_NUMBER;i++) {
		_map[i].clear();
	}

	_map.clear();
}

void Hashmap::Add(int head, int tail, int middle) {
	if (head > tail) {
		std::swap(head, tail);
	}

	int x = abs(head * (head | tail) + tail * (head | tail)) % MAGIC_NUMBER;

	_map[x].push_back (std::make_pair (std::make_pair (head, tail), middle));
}

int Hashmap::Get(int head, int tail) {
	if (head > tail) {
		std::swap(head, tail);
	}

	int x = abs(head * (head | tail) + tail * (head | tail)) % MAGIC_NUMBER;

	for (std::size_t i=0;i<_map[x].size();i++) {
		if (_map[x][i].first.first == head &&
			_map[x][i].first.second == tail) {
			return _map[x][i].second;
		}
	}

	return -1;
}