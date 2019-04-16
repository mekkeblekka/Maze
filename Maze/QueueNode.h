#pragma once
#include "Node.h"
template <typename T>
class QueueNode
{
public:
	QueueNode(Node<T>* _node);
	~QueueNode();
private:
	Node<T>* node;
	vector<Node<T>*> path;

	friend class Graph<T>;
};

