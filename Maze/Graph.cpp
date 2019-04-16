#include "Graph.h"

#include <queue>


template <typename T>
Graph<T>::Graph()
{
}

template <typename T>
Graph<T>::~Graph()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i]->nbs.size(); j++)
		{
			delete nodes[i]->nbs[j];
		}
		delete nodes[i];
	}
}

template<typename T>
void Graph<T>::add_node(T val)
{
	Node<T>* n = new Node<T>(val);
	nodes.push_back(n);
}

template<typename T>
void Graph<T>::add_edge(T val1, T val2, int weight)
{
	add_edge(get_node(val1), get_node(val2), weight);
}

template<typename T>
void Graph<T>::add_two_way_edge(T val1, T val2, int weight)
{
	//Lager to kanter, en for hver retning
	Node<T>* n1 = get_node(val1);
	Node<T>* n2 = get_node(val2);
	add_edge(n1, n2, weight);
	add_edge(n2, n1, weight);
}

template<typename T>
void Graph<T>::remove_node(T val)
{
	remove_node(get_node(val));
}

template<typename T>
void Graph<T>::remove_edge(T val1, T val2)
{
	remove_edge(get_node(val1), get_node(val2));
}

template<typename T>
void Graph<T>::print_graph()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		cout << "<" <<get<0>(nodes[i]->val) << ", " << get<1>((nodes[i]->val)) << "> ";
		for (int j = 0; j < nodes[i]->nbs.size(); j++)
		{
			if(nodes[i]->nbs[j]->to != nodes[i]) cout << "<" <<get<0>(nodes[i]->nbs[j]->to->val) << ", " << get<1>(nodes[i]->nbs[j]->to->val) << "> ";
			else cout << "<" << get<0>(nodes[i]->nbs[j]->from->val) << ", " << get<1>(nodes[i]->nbs[j]->from->val) << "> ";
		}
		cout << endl;
	}
}

template<typename T>
Node<T>* Graph<T>::get_node(T val)
{
	for (auto n : nodes) if (n->val == val) return n;
	return nullptr;
}

template<typename T>
bool Graph<T>::node_in_vector(Node<T>* n, vector<Node<T>*>& v)
{
	for (auto i : v) if (i == n) return true;
	return false;
}

template<typename T>
void Graph<T>::add_edge(Node<T>* n1, Node<T>* n2, int weight)
{
	//Sjekker om kanten finnes fra før, endrer i så fall bare vekt. 
	/*for (int i = 0; i < n1->nbs.size())
	{
		if (n1->nbs[i]->to == n2);
	}
	for (auto e : n1->nbs)
	{
		if (e->to == n2)
		{
			e->weight = weight;
		}
	}*/
	Edge<T>* e = new Edge<T>(n1, n2, weight);
	n1->nbs.push_back(e);
}

template<typename T>
void Graph<T>::remove_node(Node<T>* n)
{
	//Fjerner først alle kantene noden peker til.
	for (int i = 0; i < n->nbs.size(); i++)
	{
		remove_edge(n, n->nbs[i]->to);
	}
	//Sjekker alle noder og fjerner kanter som peker til noden n
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i]->nbs.size(); j++)
		{
			if (nodes[i]->nbs[j]->to == n) nodes[i]->nbs.remove(nodes[i]->nbs.begin() + i);
		}
	}
}

template<typename T>
void Graph<T>::remove_edge(Node<T>* n1, Node<T>* n2)
{
	//Fjerner kanten fra node n1 til n2
	//Sletter hele kanten
	for (int i = 0; i < n1->nbs.size(); i++)
	{
		if (n1->nbs[i]->to == n2)
		{
			Edge<T>* temp = n1->nbs[i];
			n1->nbs.remove(n1->nbs.begin() + i);
			delete temp;
			return;
		}
	}
}

template<typename T>
bool Graph<T>::edge_in_vector(Edge<T>* e, vector<Edge<T>*>& v)
{
	for (auto i : v) if (i == e) return true;
	return false;
}

//Algoritmer:
//Dijkstra:


//Public Dijkstra-funksjon
template<typename T>
Graph<T>* Graph<T>::dijkstra()
{
	vector<Queue_Node<T>*> queue;
	Node<T>* start = start_stop[0];
	queue.push_back(new Queue_Node<T>(start, true));

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i] != start) queue.push_back(new Queue_Node<T>(nodes[i]));
	}
	Node<T>* target = start_stop[1];
	vector<Node<T>*> visited;
	return dijkstra(queue, target, visited);
}

//Hovedfunksjon for Dijkstra
template<typename T>
Graph<T>* Graph<T>::dijkstra(vector<Queue_Node<T>*> queue, Node<T>* target, vector<Node<T>*> visited)
{
	Node<T>* current;
	Node<T>* current_neighbour;
	Queue_Node<T>* neighbour_in_queue;
	vector<Queue_Node<T>*> path;

	while (!queue.empty())
	{
		sort(queue.begin(), queue.end(), Queue_Node<T>::Less_Dist);
		current = queue.front()->get_node();
		for (int i = 0; i < current->nbs.size(); i++)
		{
			if (current->nbs[i]->to == current) current_neighbour = current->nbs[i]->from;
			else current_neighbour = current->nbs[i]->to;
			if (node_in_vector(current_neighbour, visited)) continue;
			neighbour_in_queue = get_queue_node(current_neighbour, queue);

			if (neighbour_in_queue->get_dist() > queue.front()->get_dist() + current->nbs[i]->weight)
			{
				neighbour_in_queue->set_dist(queue.front()->get_dist() + current->nbs[i]->weight);
				neighbour_in_queue->set_path(current);
			}
		}
		path.push_back(queue.front());
		queue.erase(queue.begin());
		visited.push_back(current);
	}
	dist_to_goal = get_queue_node(target, path)->get_dist();
	return graph_from_path(path, target);
}
//Returnerer riktig kø_node
template <typename T>
Queue_Node<T>* Graph<T>::get_queue_node(Node<T>* n, vector<Queue_Node<T>*>& q)
{
	for (int i = 0; i < q.size(); i++)
	{
		if (q[i]->get_node() == n) return q[i];
	}
	return nullptr;
}
//Lager en ny graf utifra en liste med kønoder.
template<typename T>
Graph<T>* Graph<T>::graph_from_path(vector<Queue_Node<T>*> q, Node<T>* target)
{
	Node<T>* current = target;
	Graph<T>* g = new Graph<T>();
	Queue_Node<T>* current_queue_node = get_queue_node(current, q);
	g->add_node(current->get_val());
	g->start_stop.push_back(g->nodes.back());
	while (current_queue_node->get_path() != nullptr)
	{
		int weight;
		for (int i = 0; i < current->nbs.size(); i++)
		{
			if (current->nbs[i]->to == current_queue_node->get_path() || current->nbs[i]->from == current_queue_node->get_path()) weight = current->nbs[i]->weight;
		}
		g->add_node(current_queue_node->get_path()->get_val());
		g->add_two_way_edge(current->get_val(), current_queue_node->get_path()->get_val(), weight);

		current = current_queue_node->get_path();
		current_queue_node = get_queue_node(current, q);
		if (current_queue_node->get_path() == nullptr) g->start_stop.push_back(g->nodes.back());
	}
	return g;
}
