#include "QueueNode.h"
#include "Node.cpp"

template <typename T>
QueueNode<T>::QueueNode(Node<T>* _node) :
	node(_node)
{
}

template <typename T>
QueueNode<T>::~QueueNode()
{
}
