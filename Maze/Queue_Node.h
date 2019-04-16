#pragma once

#include "Edge.h"
#include "Edge.cpp"

template <typename T>
class Queue_Node
{
public:
	Queue_Node(Node<T>* _node);
	Queue_Node(Node<T>* _node, bool is_start);
	~Queue_Node();
	Node<T>* get_path();
	unsigned long long get_dist();
	Node<T>* get_node();
	void set_path(Node<T>* n);
	void set_dist(unsigned long long d);
	static bool Less_Dist(Queue_Node<T>* a, Queue_Node<T>* b)
	{
		return a->get_dist() < b->get_dist();
	}

private:
	Node<T>* path;
	unsigned long long dist;
	Node<T>* node;
};