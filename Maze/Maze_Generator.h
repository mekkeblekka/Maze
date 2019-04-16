#pragma once
#include "Graph.h"
#include "Maze_IMG.h"
#include "Wall.h"
#include "lodepng.h"



class Maze_Generator
{
public:
	virtual ~Maze_Generator();

	void generate(const char* filename);

protected:
	Maze_Generator(int _size);
	int size;
	vector<vector<int>>* grid;
	vector<Wall>* wall_queue;
	virtual void generate_maze() = 0;
	void encodeOneStep(const char* filename, std::vector<unsigned char>& image);
	int get_upper(int row, int collumn);
	int get_lower(int row, int collumn);
	int get_left(int row, int collumn);
	int get_right(int row, int collumn);
	vector<unsigned char>* grid_to_buffer();
	bool check_wall(Wall w);
	void add_neighbour_wall(int row, int collumn);
	bool wall_in_queue(int row, int collumn);
	void add_entrances();
};
/*

class Maze_Generator
{
public:
	Maze_Generator(int _size);
	~Maze_Generator();
	void generate(const char* filename, bool* ready);
private:
	int size;
	vector<vector<int>>* grid;
	void generate_maze();
	void add_nodes();
	void encodeOneStep(const char * filename, std::vector<unsigned char>& image);
	int get_upper(int row, int collumn);
	int get_lower(int row, int collumn);
	int get_left(int row, int collumn);
	int get_right(int row, int collumn);
	vector<Wall>* wall_queue;
	bool check_wall(Wall w);
	void add_neighbour_wall(int row, int collumn);
	
	bool wall_in_queue(int row, int collumn);
	void add_entrances();
};

*/