#include "Queue_Node.h"


template<typename T>
Queue_Node<T>::Queue_Node(Node<T>* _node) :
	node(_node), dist(LLONG_MAX)
{
}

template<typename T>
Queue_Node<T>::Queue_Node(Node<T>* _node, bool is_start) :
	node(_node), dist(0), path(nullptr)
{
}

template<typename T>
Queue_Node<T>::~Queue_Node()
{
}

template<typename T>
Node<T>* Queue_Node<T>::get_path()
{
	return path;
}

template<typename T>
unsigned long long Queue_Node<T>::get_dist()
{
	return dist;
}

template<typename T>
Node<T>* Queue_Node<T>::get_node()
{
	return node;
}

template<typename T>
void Queue_Node<T>::set_path(Node<T>* n)
{
	path = n;
}

template<typename T>
void Queue_Node<T>::set_dist(unsigned long long d)
{
	dist = d;
}

