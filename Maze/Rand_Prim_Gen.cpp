#include "Rand_Prim_Gen.h"



Rand_Prim_Gen::Rand_Prim_Gen(int _size) :
	Maze_Generator(_size)
{}


Rand_Prim_Gen::~Rand_Prim_Gen()
{	
}


void Rand_Prim_Gen::generate_maze()
{
	int row_start = rand() % (size - 1);
	int col_start = rand() % (size - 1);
	(*grid)[row_start][col_start] = 0;
	add_neighbour_wall(row_start, col_start);


	while (!wall_queue->empty())
	{
		int index = rand() % wall_queue->size();
		Wall current_wall = wall_queue->at(index);
		if (check_wall(current_wall))
		{
			(*grid)[current_wall.row][current_wall.collumn] = 0;
			add_neighbour_wall(current_wall.row, current_wall.collumn);
		}
		wall_queue->erase(wall_queue->begin() + index);
	}
}
