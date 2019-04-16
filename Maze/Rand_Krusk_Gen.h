/*#pragma once
#include "Maze_Generator.h"
#include "Cell.h"
#include <set>
#include "Wall.h"
#include <tuple>


class Rand_Krusk_Gen : public Maze_Generator
{
public:
	Rand_Krusk_Gen(int _size);
	~Rand_Krusk_Gen();
private:

	vector<vector<Cell>>* cells;
	
	void generate_maze();
	void merge_sets();
	bool check_different_sets;
	Cell get_right(Wall current);
	Cell get_left(Wall current);
	Cell get_upper(Wall current);
	Cell get_lower(Wall current);

	Cell cell_in_sub(Cell, vector<Cell>& v);

};*/
