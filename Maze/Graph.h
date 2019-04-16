#pragma once
#include "Queue_Node.h"
#include "Queue_Node.cpp"
template <typename T>
class Graph
{
public:
	Graph();
	~Graph();
	void add_node(T val);
	void add_edge(T val1, T val2, int weight);
	void add_two_way_edge(T val1, T val2, int weight);
	void remove_node(T val);
	void remove_edge(T val1, T val2);
	//ostream& print_graph(ostream& os);
	void print_graph();

	//Algoritmer:
	Graph<T>* dijkstra();
	long dist_to_goal;
	
protected:
	vector<Node<T>*> nodes;
	Node<T>* get_node(T val);
	vector<Node<T>*> start_stop;
	bool node_in_vector(Node<T>* n, vector<Node<T>*>& v);
	void add_edge(Node<T>* n1, Node<T>* n2, int weight);
	void remove_node(Node<T>* n);
	void remove_edge(Node<T>* n1, Node<T>* n2);
	bool edge_in_vector(Edge<T>* e, vector <Edge<T>*>& v);
	
	//Algoritmer:
	Queue_Node<T>* get_queue_node(Node<T>* n, vector<Queue_Node<T>*>& q);
	Graph<T>* dijkstra(vector<Queue_Node<T>*> queue, Node<T>* target, vector<Node<T>*> visited);
	Graph<T>* graph_from_path(vector<Queue_Node<T>*> q, Node<T>* target);



	friend class Maze_IMG;
	friend class Maze_Solver;
};



