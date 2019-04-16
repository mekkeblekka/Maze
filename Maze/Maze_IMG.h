#pragma once
#include "lodepng.h"
#include <vector>
#include <iostream>
#include "gif.h"
#include <thread>
#include <windows.h>
#include <mutex>
#include <chrono>



/*
Bruk labyrinter herfra: http://hereandabove.com/maze/mazeorig.form.html
*/


using namespace std;

class Maze_IMG
{
public:
	//Konst tar et filnavn, husk ".png"
	Maze_IMG(const char* _file);
	~Maze_IMG();
	//Gir brukeren A x B grid, hvor løsning plottes. 
	//0: blank, 1: vegg, 2: sti
	vector<vector<int>> get_grid();
	//printer grid
	void print_saved_grid();
	//Bruker sender med løsning, lagres til "solved.png"
	void set_grid_and_save(const char* filename, vector<vector<int>> grid);
	
	//GIfgreier
	GifWriter* writer = new GifWriter();
	void initGif(const char* output_gif);
	void pass_gif_frame(vector<vector<int>> grid);
	void close_gif();
	vector<unsigned char>* resample(vector<unsigned char>* buf, int nWidth, int nHeight);
	
private:
	int gif_size = 500;
	int gif_speed = 4;
	int SCREENX = GetSystemMetrics(SM_CXSCREEN);
	int SCREENY = GetSystemMetrics(SM_CYSCREEN);
	unsigned HEIGHT;
	unsigned WIDTH;
	const char* input_filename;
	vector<vector<int>> GRID;
	vector<unsigned char> buffer;
	vector<unsigned char> resized_buffer;
	unsigned resize_height;
	unsigned resize_width;

	//Fra eksempel, returnerer vector med RGBA verdier.
	//Setter også høyde og bredde
	void decodeOneStep(const char* filename);
	//Oversetter RGBA til grid med enere og nuller
	void image_to_grid();
	//Oversetter tilbake til RGBA
	vector<unsigned char>* grid_to_buffer(vector<vector<int>> grid);
	//Fra eksempelet, encoder solved.png.
	void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);
	void crop_edge();


	//void threading();
	//void image_to_grid_threading(int thread_number, int number_of_threads);
};

