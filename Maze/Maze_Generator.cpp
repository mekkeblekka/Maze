#include "Maze_Generator.h"



Maze_Generator::Maze_Generator(int _size):
	size(_size)
{
	//Graph<tuple<int, int>>* graph = new Graph < tuple<int, int>>();
	grid = new vector<vector<int>>();
	grid->resize(size, vector<int>(size));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			(*grid)[i][j] = 1;
		}
	}
	wall_queue = new vector<Wall>();

}


Maze_Generator::~Maze_Generator()
{
	delete wall_queue;
	delete grid;
	
}

void Maze_Generator::generate(const char * filename)
{
	cout << "Begynt " << endl;

	generate_maze();
	add_entrances();
	encodeOneStep(filename, *grid_to_buffer());
	cout << "Ferdig!" << endl;
	
}


/*
void Maze_Generator::generate_maze()
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
		wall_queue->erase(wall_queue->begin()+index);
	}
}
*/

int Maze_Generator::get_upper(int row, int collumn)
{
	if (row - 1 < 0) return 0;
	return (*grid)[row-1][collumn];
}
int Maze_Generator::get_lower(int row, int collumn)
{
	if (row + 1 >= size) return 0;
	return (*grid)[row + 1][collumn];
}
int Maze_Generator::get_left(int row, int collumn)
{
	if (collumn - 1 < 0) return 0;
	return (*grid)[row][collumn-1];
}
int Maze_Generator::get_right(int row, int collumn)
{
	if (collumn + 1 >=  size) return 0;
	return (*grid)[row][collumn+1];
}

bool Maze_Generator::check_wall(Wall w)
{


	//if (get_upper(w.row, w.collumn) + get_lower(w.row, w.collumn) == 1) return true;
	//if (get_left(w.row, w.collumn) + get_right(w.row, w.collumn) == 1) return true;
	//return false;
	
	int count = 0; 
	count += get_upper(w.row, w.collumn);
	count += get_lower(w.row, w.collumn);
	count += get_left(w.row, w.collumn);
	count += get_right(w.row, w.collumn);

	if (count > 2) return true;
	return false;
	
}

void Maze_Generator::add_neighbour_wall(int row, int collumn)
{
	if (get_upper(row, collumn) == 1 && !wall_in_queue(row-1, collumn)) wall_queue->push_back(Wall(row-1, collumn));
	if (get_lower(row, collumn) == 1 && !wall_in_queue(row+1, collumn)) wall_queue->push_back(Wall(row + 1, collumn));
	if (get_left(row, collumn) == 1 && !wall_in_queue(row, collumn-1)) wall_queue->push_back(Wall(row, collumn - 1));
	if (get_right(row, collumn) == 1 && !wall_in_queue(row, collumn+1)) wall_queue->push_back(Wall(row, collumn + 1));
}

//Lager et png av RGBA-vector
void Maze_Generator::encodeOneStep(const char * filename, std::vector<unsigned char>& image)
{
	//Encode the image
	unsigned error = lodepng::encode(filename, image, size, size);
	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

vector<unsigned char>* Maze_Generator::grid_to_buffer()
{
	vector<unsigned char>* buf = new vector<unsigned char>();
	//cout << "Buffer før grid_to_buffer: " << buffer.size() << endl;
	int row = 0;
	int collumn = 0;
	//cout << "Kom inn hit." << endl;
	int path = 0;
	for (int i = 0; i < size*size; i++)
	{
		path = i % 255;
		int val = (*grid)[row][collumn];
		switch (val)
		{
		case 0:
		{
			buf->push_back(255);
			buf->push_back(255);
			buf->push_back(255);
			buf->push_back(255);
			break;
		}
		case 1:
		{
			buf->push_back(0);
			buf->push_back(0);
			buf->push_back(0);
			buf->push_back(255);
			break;
		}
		case 2:
		{

			buf->push_back(path);
			buf->push_back(165);
			buf->push_back(33);
			buf->push_back(240);
		}
		}
		collumn++;
		if (collumn % size == 0)
		{
			collumn = 0;
			row++;
		}
	}
	return buf;
}

bool Maze_Generator::wall_in_queue(int row, int collumn)
{
	for (auto i : (*wall_queue))
	{
		if (i.row == row && i.collumn == collumn) return true;
	}
	return false;
}

void Maze_Generator::add_entrances()
{
	while (true)
	{
		int row = 0;
		int collumn = 1;
		if ((*grid)[row + 1][collumn] == 0)
		{
			(*grid)[row][collumn] = 0;
			break;
		}
		collumn++;
	}
	while (true)
	{
		int row = size - 1;
		int collumn = size - 2;

		if ((*grid)[row - 1][collumn] == 0)
		{
			(*grid)[row][collumn] = 0;
			break;
		}
		collumn--;
	}


}

