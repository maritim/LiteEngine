#include <vector>
#include <algorithm>

#include "Graph.h"

Graph::Graph(int capacity) :
	_list (),
	_facesList ()
{
	AddCapacity (capacity);
}

void Graph::AddEdge(int head, int tail)
{
	AddCapacity (std::max(head, tail));

	for (std::vector<int>::iterator i=_list[head].begin();i<_list[head].end();i++) {
		if (*i == tail) {
			return ;
		}
	}

	_list[head].push_back(tail);
	_list[head].push_back(head);
}

void Graph::AddFace(int node, int face)
{
	AddCapacity (node);

	for (std::vector<int>::iterator i=_facesList[node].begin();i<_facesList[node].end();i++) {
		if (*i == face) {
			return ;
		}
	}

	_facesList[node].push_back(face);
}

void Graph::RemoveFace(int node, int face)
{
	AddCapacity (node);

	for (std::vector<int>::iterator i=_list[node].begin();i<_list[node].end();i++) {
		if (*i == face) {
			int aux = _list[node].back();
			_list[node].back() = *i;
			*i = aux;

			_list[node].pop_back ();

			return ;
		}
	}
}

void Graph::RemoveEdge(int head, int tail)
{
	AddCapacity (std::max(head, tail));

	RemoveDirectEdge (head, tail);
	RemoveDirectEdge (tail, head);
}

bool Graph::ExistEdge(int head, int tail)
{
	AddCapacity (std::max(head, tail));

	for (std::vector<int>::iterator i=_list[head].begin();i<_list[head].end();i++) {
		if (*i == tail) {
			return true;
		}
	}

	return false;
}

bool Graph::ExistFaceConnection (int node, int face)
{
	AddCapacity (node);

	for (std::vector<int>::iterator i=_facesList[node].begin ();i<_facesList[node].end ();i++) {
		if (*i == face) {
			return true;
		}
	}

	return false;
}

std::vector<int> &Graph::Neighbours(int node)
{
	AddCapacity (node);

	return _list[node];
}

std::vector<int> &Graph::NeighbourFaces(int node)
{
	AddCapacity (node);

	return _facesList[node];
}

void Graph::RemoveDirectEdge(int head, int tail)
{
	for (std::vector<int>::iterator i=_list[head].begin();i<_list[head].end();i++) {
		if (*i == tail) {
			int aux = _list[head].back();
			_list[head].back() = *i;
			*i = aux;

			_list[head].pop_back ();

			return ;
		}
	}
}

int Graph::NodeCount(void) const
{
	return _list.size();
}

void Graph::AddCapacity(int limit)
{
	limit -= _list.size() - 1;
	while(limit > 0) {
		_list.push_back(std::vector<int>());
		_facesList.push_back(std::vector<int>());
		-- limit;
	}
}

Graph::~Graph ()
{
}