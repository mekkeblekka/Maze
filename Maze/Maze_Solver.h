#pragma once
#include "Maze_IMG.h"
#include "Graph.h"


class Maze_Solver
{
public:
	Maze_Solver(const char* _input_file, const char* _output_file, const char* _gif_output_file);
	~Maze_Solver();
	void solve();
	int nu_of_nodes_created();
	unsigned long long distance;
private:
	const char* input_file;
	const char* output_file;
	const char* gif_output_file;
	Maze_IMG* maze;
	vector<vector<int>> grid;
	Graph<tuple<int, int>>* graph;
	GifWriter* gifwriter;
	void create_nodes();
	int get_above(int row, int collumn);
	int get_left(int row, int collumn);
	int get_right(int row, int collumn);
	int get_under(int row, int collumn);
	bool check_corner(int row, int collumn);
	bool check_cross(int row, int collumn);
	bool check_start_end(int row, int collumn);
	void make_node(int row, int collumn, bool entrance);
	void update_grid(Graph<tuple<int, int>>* g);
	void plot_path_to_grid(Graph<tuple<int, int>>* g);
	bool node_in_vector(Node<tuple<int, int>>* n, vector < Node<tuple<int, int>>*>& v);
	//void plot_queue_to_grid(vector<Queue_Node<tuple<int, int>)
};

