#include "Maze_Solver.h"
#include "Graph.cpp"

Maze_Solver::Maze_Solver(const char* _input_file, const char* _output_file, const char* _gif_output_file) :
	input_file(_input_file), output_file(_output_file), gif_output_file(_gif_output_file)
{
	maze = new Maze_IMG(input_file);
	grid = maze->get_grid();
	graph = new Graph<tuple<int, int>>();
	
}

Maze_Solver::~Maze_Solver()
{
}

//Gjør alt source main gjorde før.
void Maze_Solver::solve()
{
	create_nodes();

	cout << "Created nodes: " << graph->nodes.size() << endl;
	cout << "Inngang: " << get<0>(graph->start_stop[0]->val) <<", " << get<1>(graph->start_stop[0]->val) << endl;
	cout << "Utgang: " << get<0>(graph->start_stop[1]->val) << ", " << get<1>(graph->start_stop[1]->val) << endl;

	Graph<tuple<int, int>>* dijkstraGraph = graph->dijkstra();
	
	plot_path_to_grid(dijkstraGraph);
	distance = graph->dist_to_goal;

	cout << "Distance =  " << graph->dist_to_goal << endl;
	maze->set_grid_and_save(output_file, grid);
	
}
int Maze_Solver::nu_of_nodes_created()
{
	return graph->nodes.size();
}
//Lager noder til grafen for hele lab.
void Maze_Solver::create_nodes()
{
	cout << "Noder skapes.." << endl;
	for (int row = 0; row < grid.size(); row++)
	{
		for (int collumn = 0; collumn < grid.size(); collumn++)
		{
			if (graph->start_stop.size() < 2)
			{
				if (check_start_end(row, collumn)) continue;
			}
			if (check_cross(row, collumn)) continue;
			check_corner(row, collumn);

		}
	}
}
//Disse funksjonene henter verdier i grid:
int Maze_Solver::get_above(int row, int collumn)
{
	if (row - 1 < 0) return -1;
	return grid[row - 1][collumn];
}
int Maze_Solver::get_left(int row, int collumn)
{
	if (collumn - 1 < 0) return -1;
	return grid[row][collumn - 1];
}
int Maze_Solver::get_right(int row, int collumn)
{
	if (collumn + 1 >= grid.size()) return -1;
	return grid[row][collumn + 1];
}
int Maze_Solver::get_under(int row, int collumn)
{
	if (row + 1 >= grid.size()) return -1;
	return grid[row + 1][collumn];
}
//Sjekker for hjørnenode
bool Maze_Solver::check_corner(int row, int collumn)
{
		if (grid[row][collumn] == 1) return false;
		if (get_above(row, collumn) == 1 && get_under(row, collumn) == 1) return false;
		if (get_left(row, collumn) == 1 && get_right(row, collumn) == 1) return false;
		
		make_node(row, collumn, false);
		return true;
}
//Sjekker for kryss-node
bool Maze_Solver::check_cross(int row, int collumn)
{
	if (grid[row][collumn] == 1) return false;
	int counter = 0;
	if (get_above(row, collumn) == 0) counter++;
	if (get_right(row, collumn) == 0) counter++;
	if (get_left(row, collumn) == 0) counter++;
	if (get_under(row, collumn) == 0) counter++;
	if (counter >= 3)
	{
		
		make_node(row, collumn, false);
		return true;
	}
	return false;
}
//Sjekker for start/slutt-node
bool Maze_Solver::check_start_end(int row, int collumn)
{
	if (((row == 0 || row == grid.size() - 1) && grid[row][collumn] == 0) || ((collumn == 0 || collumn == grid.size() - 1) && grid[row][collumn] == 0))
	{
		
		make_node(row, collumn, true);
		return true;
	}
	return false;
}
//Lager node og legger til kanter
void Maze_Solver::make_node(int row, int collumn, bool entrance)
{
	tuple<int, int> t;
	t = make_tuple(row, collumn);
	if (!(graph->get_node(t) == nullptr)) return;
	graph->add_node(t);
	//cout << "Made node: " << get<0>(t) << ", " << get<1>(t) << endl;
	Node<tuple<int, int>>* added_node = graph->get_node(t);
	if (entrance) graph->start_stop.push_back(added_node);
	//Sjekker naboer rett nedover
	for (int i = row+1; i < grid.size(); i++)
	{
		if (grid[i][collumn] == 1) break;
		tuple<int, int> temp;
		temp = make_tuple(i, collumn);
		if (graph->get_node(temp) != nullptr)
		{
			graph->add_two_way_edge(temp, t, i-row);
			break;
		}
	}
	//Sjekker naboer rett oppover
	for (int i = row-1; i >= 0; i--)
	{
		if (grid[i][collumn] == 1) break;
		tuple<int, int> temp;
		temp = make_tuple(i, collumn);
		if (graph->get_node(temp) != nullptr)
		{
			graph->add_two_way_edge(temp, t, row-i);
			break;
		}
	}
	//Sjekker naboer til venstre
	for (int i = collumn-1; i >= 0; i--)
	{
		if (grid[row][i] == 1) break;
		tuple<int, int> temp;
		temp = make_tuple(row, i);
		if (graph->get_node(temp) != nullptr)
		{
			graph->add_two_way_edge(temp, t, collumn-i);
			break;
		}
	}
	//Sjekker naboer til høyre
	for (int i = collumn+1; i < grid.size(); i++)
	{
		if (grid[row][i] == 1) break;
		tuple<int, int> temp;
		temp = make_tuple(row, i);
		if (graph->get_node(temp) != nullptr)
		{
			graph->add_two_way_edge(temp, t, i-collumn);
			break;
		}
	}
}

//Oppdaterer medlemsvariabel 'grid'
void Maze_Solver::update_grid(Graph<tuple<int, int>>* g)
{
	for (int i = 0; i < g->nodes.size(); i++)
	{
		tuple<int, int> t = g->nodes[i]->get_val();
		grid[get<0>(t)][get<1>(t)] = 2;
	}
}
//Plotter en graf returnert av Dijkstra til medlemsvariabel 'grid'
void Maze_Solver::plot_path_to_grid(Graph<tuple<int, int>>* g)
{
	cout << "Begynt på path to grid" << endl;
	vector<Node<tuple<int, int>>*> visited;
	Node<tuple<int, int>>* current = g->start_stop[1];
	Node<tuple<int, int>>* next = nullptr;
	maze->initGif(gif_output_file);
	char out = 'a';
	char buf[6];
	buf[0] = out;
	buf[1] = '.';
	buf[2] = 'p';
	buf[3] = 'n';
	buf[4] = 'g';
	buf[5] = '\0';
	vector<thread*> threads;

	
	while (current != g->start_stop[0])
	{
		
		
		for (int i = 0; i < current->nbs.size(); i++)
		{
			if (current->nbs[i]->from != current && !node_in_vector(current->nbs[i]->from, visited))
			{
				next = current->nbs[i]->from;
			}
			else if (current->nbs[i]->to != current && !node_in_vector(current->nbs[i]->to, visited))
			{
				next = current->nbs[i]->to;
			}
		}
		visited.push_back(current);

		//HER SKAL DU MERKE KOORDINATER MELLOM NABONODENE.
		int cur_row = get<0>(current->val);
		int cur_col = get<1>(current->val);
		int nex_row = get<0>(next->val);
		int nex_col = get<1>(next->val);
		int limit;
		int index;
		//På samme rad
		

		//threads.clear();
		if (cur_row == nex_row && cur_col < nex_col)
		{
			
			index = cur_col;
			limit = nex_col;
			while (index <= limit)
			{
				grid[cur_row][index] = 2;
				//Dette funker!!! Fortsett i morgen!!!
				if (index % 2 == 0)
				{
					thread* t1 = new thread(&Maze_IMG::pass_gif_frame, maze, grid);
					threads.push_back(t1);
				}
				else maze->pass_gif_frame(grid);
				index++;
				buf[0]++;
			}

		}
		else if (cur_row == nex_row && cur_col > nex_col)
		{
			index = cur_col;
			limit = nex_col;
			while (index >= limit)
			{
				grid[cur_row][index] = 2;
				if (index % 2 == 0)
				{
					thread* t1 = new thread(&Maze_IMG::pass_gif_frame, maze, grid);
					threads.push_back(t1);
				}
				else maze->pass_gif_frame(grid);
				index--;
				buf[0]++;
			}
		}
		else if (cur_col == nex_col && cur_row < nex_row)
		{
			index = cur_row;
			limit = nex_row;
			while (index <= limit)
			{
				grid[index][cur_col] = 2;
				if (index % 2 == 0)
				{
					thread* t1 = new thread(&Maze_IMG::pass_gif_frame, maze, grid);
					threads.push_back(t1);
				}
				else maze->pass_gif_frame(grid);
				
				index++;
				buf[0]++;
			}
		}
		else if (cur_col == nex_col && cur_row > nex_row)
		{
			index = cur_row;
			limit = nex_row;
			while (index >= limit)
			{
				grid[index][cur_col] = 2;
				if (index % 2 == 0)
				{
					thread* t1 = new thread(&Maze_IMG::pass_gif_frame, maze, grid);
					threads.push_back(t1);
				}
				else maze->pass_gif_frame(grid);
				index--;
				buf[0]++;
			}
		}
		current = next;
	}
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i]->join();
	}
}
//Sjekker om en node er i en vektor
bool Maze_Solver::node_in_vector(Node<tuple<int, int>>* n, vector<Node<tuple<int, int>>*>& v)
{
	for (auto i : v) if (i == n) return true;
	return false;
}
