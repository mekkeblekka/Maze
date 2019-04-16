#include "Maze_IMG.h"

mutex gif_frame_lock;


Maze_IMG::Maze_IMG(const char* _file) :
	input_filename(_file)
{
}


Maze_IMG::~Maze_IMG()
{
}

vector<vector<int>> Maze_IMG::get_grid()
{
	image_to_grid();
	//threading();
	//cout << "Hentet grid\nStorrelse: " << HEIGHT << " x " << WIDTH << "\nPiksler: " << HEIGHT*WIDTH << endl;
	return GRID;
}

void Maze_IMG::print_saved_grid()
{
	cout << "Current saved grid: " << endl;
	for (int i = 0; i < GRID.size(); i++)
	{
		for (int j = 0; j < GRID.size(); j++)
		{
			cout << GRID[i][j];
		}
		cout << endl;
	}
}
//Tar et grid, og lagrer til .png
void Maze_IMG::set_grid_and_save(const char* filename, vector<vector<int>> grid)
{
	GRID = grid;
	vector<unsigned char>* buf = grid_to_buffer(grid);
	cout << "Buffer oppdatert" << endl;
	vector<unsigned char>* resized_image = resample(buf, gif_size, gif_size);
	encodeOneStep(filename, *resized_image, gif_size, gif_size);
}
//Initialiserer gif
void Maze_IMG::initGif(const char* output_gif)
{
	writer = new GifWriter();
	GifBegin(writer, output_gif, gif_size, gif_size, 2);
}
//Sender et frame til gif-løsning
void Maze_IMG::pass_gif_frame(vector<vector<int>> grid)
{
	GRID = grid;
	vector<unsigned char>* buf = grid_to_buffer(grid);
	vector<unsigned char>* resized_buffer = resample(buf, gif_size, gif_size);

	static_assert(std::is_same<unsigned char, uint8_t>::value, "uint8_t is not unsigned char");
	uint8_t* data = resized_buffer->data();
	
	gif_frame_lock.lock();
	GifWriteFrame(writer, data, gif_size, gif_size, 2);
	gif_frame_lock.unlock();

	delete buf;
	delete resized_buffer;
}
//Lukker og lagrer gif_løsning
void Maze_IMG::close_gif()
{
	GifEnd(writer);
}

//Endrer størrelse på buffer
vector<unsigned char>* Maze_IMG::resample(vector<unsigned char>* buf, int nWidth, int nHeight)
{
	//if (buffer.size() == 0) return false;
	//resized_buffer.clear();
	vector<unsigned char>* resized_buffer = new vector<unsigned char>(nWidth*nHeight*4);
	//for (int i = 0; i < nWidth * nHeight * 4; i++) resized_buffer.push_back(-1);

	double scaleWidth = (double)nWidth / (double)WIDTH;
	double scaleHeight = (double)nHeight / (double)HEIGHT;

	for (int cy = 0; cy < nHeight; cy++)
	{
		for (int cx = 0; cx < nWidth; cx++)
		{
			int pixel = (cy * (nWidth * 4)) + (cx * 4);
			int nearestMatch = (((int)(cy / scaleHeight) * (WIDTH * 4)) + ((int)(cx / scaleWidth) * 4));

			(*resized_buffer)[pixel] = (*buf)[nearestMatch];
			(*resized_buffer)[pixel + 1] = (*buf)[nearestMatch + 1];
			(*resized_buffer)[pixel + 2] = (*buf)[nearestMatch + 2];
			(*resized_buffer)[pixel + 3] = (*buf)[nearestMatch + 3];
		}
	}
	return resized_buffer;
}
//Tar et png-bilde, og returnerer RGBA-vector
void Maze_IMG::decodeOneStep(const char * filename)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	//decode
	unsigned error = lodepng::decode(buffer, width, height, filename);
	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	WIDTH = width;
	HEIGHT = height;
	GRID.resize(HEIGHT, vector<int>(WIDTH));
}
//Konverterer RGBA-vector til 2D-vector (grid)
//0 er vei, 1 er vegg, 2 er løsning
void Maze_IMG::image_to_grid()
{
	decodeOneStep(input_filename);

	int row = 0; 
	int collumn = 0;
	for (int i = 0; i < buffer.size(); i += 4)
	{
		if (buffer[i] == 255) GRID[row][collumn] = 0;
		else GRID[row][collumn] = 1;
		collumn++;
		if (collumn % WIDTH == 0)
		{
			collumn = 0;
			row++;
		}
	}
	if (GRID[0][0] == 0 && GRID[0][1] == 0) crop_edge();
}
//Tar 2d grid, og returnerer peker til RGBA-vector
vector<unsigned char>* Maze_IMG::grid_to_buffer(vector<vector<int>> grid)
{
	vector<unsigned char>* buf = new vector<unsigned char>();
	//cout << "Buffer før grid_to_buffer: " << buffer.size() << endl;
	buffer.clear();
	int row = 0;
	int collumn = 0;
	//cout << "Kom inn hit." << endl;
	int path = 0;
	for (int i = 0; i < WIDTH*HEIGHT; i++)
	{
		path = i % 255;
		int val = grid[row][collumn];
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
		if (collumn % WIDTH == 0)
		{
			collumn = 0;
			row++;
		}
	}
	return buf;
}

//Lager et png av RGBA-vector
void Maze_IMG::encodeOneStep(const char * filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);	
	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
//Fjerner ramma på labyrint, da nettsiden har whitspaces langs kantene.
void Maze_IMG::crop_edge()
{
	GRID.erase(GRID.begin());
	GRID.erase(GRID.end() - 1);
	for (int i = 0; i < GRID.size(); i++)
	{
		GRID[i].erase(GRID[i].begin());
		GRID[i].erase(GRID[i].end() - 1);
	}
	HEIGHT -= 2;
	WIDTH -= 2;
}