/*#include "Rand_Krusk_Gen.h"

Rand_Krusk_Gen::Rand_Krusk_Gen(int _size) :
	Maze_Generator(_size)
{
	cells = new vector<vector<Cell>>();
	for (int row = 0; row < grid->size(); row++)
	{
		for (int collumn = 0; collumn < grid->size(); collumn++)
		{
			wall_queue->push_back(Wall(row, collumn));

			vector<Cell>* a = new vector<Cell>();
			a->push_back(Cell(row, collumn));
			cells->push_back(*a);

		}
	}
	for (int i = 0; i < size*size; i++)
	{
		
	}
}

Rand_Krusk_Gen::~Rand_Krusk_Gen()
{
	delete cells;
	
}


void Rand_Krusk_Gen::generate_maze()
{
	while (!walls->empty())
	{

	}
}

Cell Rand_Krusk_Gen::get_right(Wall current)
{
	for (int i = 0; i < cells->size(); i++)
	{
		if(cells[i])
	}
}

Cell Rand_Krusk_Gen::cell_in_sub(vector<Cell>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if(v[i] == )
	}
}
*/