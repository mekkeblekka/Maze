#include "Node.h"


template <typename T>
Node<T>::Node(T _val):
	val(_val)
{
}

template <typename T>
Node<T>::~Node()
{
}

template<typename T>
T Node<T>::get_val()
{
	return val;
}
