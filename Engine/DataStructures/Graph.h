/*************************************************************************** 
  Graph
 ***************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph
{
private:
	std::vector<std::vector<int> > _list;
	std::vector<std::vector<int> > _facesList;

public:
	Graph(int capacity);
	void AddEdge(int head, int tail);
	bool ExistEdge(int head, int tail);
	bool ExistFaceConnection(int node, int face);
	void AddFace(int node, int face);
	void RemoveEdge(int head, int tail);
	void RemoveFace(int node, int face);
	std::vector<int> &Neighbours(int node);
	std::vector<int> &NeighbourFaces(int node);
	int NodeCount(void) const;
	~Graph();

private:
	void AddCapacity(int limit);
	void RemoveDirectEdge(int head, int tail);
};

#endif