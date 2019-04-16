#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <queue>

using namespace std;

template <typename T>
class Edge;
template <typename T>
class Graph;
class Maze_IMG;
class Maze_Solver;

template <typename T>
class Node
{
public:
	Node(T val);
	~Node();
	T get_val();

private:
	T val;
	vector <Edge<T>*> nbs;

	friend class Edge<T>;
	friend class Graph<T>;
	friend class Maze_IMG;
	friend class Maze_Solver;

};





