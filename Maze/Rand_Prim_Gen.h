#pragma once
#include "Maze_Generator.h"
class Rand_Prim_Gen : public Maze_Generator
{
public:
	Rand_Prim_Gen(int size);
	~Rand_Prim_Gen();

private:
	void generate_maze();
};

