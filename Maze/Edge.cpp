#include "Edge.h"



template <typename T>
Edge<T>::Edge(Node<T>* _from, Node<T>* _to, int _weight):
	from(_from), to(_to), weight(_weight)
{
}

template <typename T>
Edge<T>::~Edge()
{
}

template<typename T>
int Edge<T>::get_weight()
{
	return weight;
}
