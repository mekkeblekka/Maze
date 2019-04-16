#pragma once
#include "Node.h"
#include "Node.cpp"

template <typename T>
class Edge
{
public:
	Edge(Node<T>* _from, Node<T>* to, int _weight);
	~Edge();
	int get_weight();
private:
	Node<T>* from;
	Node<T>* to;
	int weight;
	friend class Graph<T>;
	friend class Maze_IMG;
	friend class Maze_Solver;
};




