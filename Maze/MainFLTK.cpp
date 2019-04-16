#include "Maze_Solver.h"
#include "Rand_Prim_Gen.h"
#include <FL/Fl.H>
#include <FL/x.H>
#include <windows.h>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Multiline_Output.H>
#include <string>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <sstream>
#include <FL/Fl_Valuator.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Value_Input.H>
#include <cmath>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_GIF_Image.H>
#include "Fl_Anim_GIF.H"
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>

/*
Lage eget bilde pr skritt, evt lage .gif animasjon.

Legge til threading

HashMap for bedre fart?

Gjør om Mazegenerator til abstract.

Opprett subklasser for forskjellige algoritmer.

*/


Maze_Solver* ms;
Maze_Generator* mg;

Fl_Output* status_output;
Fl_Value_Output* distance_output;
Fl_Value_Output* nodes_created_output;
Fl_Value_Output* time_spent_output;

Fl_Choice* algorithm_chooser;

Fl_Input* filename_input;
Fl_Input* filename_output;
Fl_Input* gif_file_output;
Fl_Input* gen_pixel_input;

Fl_Button* solve_button;
Fl_Button* gen_button;

Fl_PNG_Image* gen_png;
Fl_Image* gen_png_full;
Fl_PNG_Image* solved_png;


Fl_Box* gen_box;
Fl_Box* solved_box;

Fl_Counter* maze_size;

bool ready;

namespace GLOBALS
{
	int SWIDTH = 0;
	int SHEIGHT = 0;
	const int BOX_WIDTH = 400;
	const int BOX_HEIGHT = 25;
	const int C_WIDTH = 200;
	const int C_HEIGHT = 25;
	const int BUTTON_WIDTH = 100;
	const int BUTTON_HEIGHT = 20;
	const int OUT_WIDTH = 100;
	const int OUT_HEIGHT = 20;

	const int BILDE1_WIDTH = 63;
	const int BILDE1_HEIGHT = 63;
	const int BILDE2_WIDTH = 500;
	const int BILDE2_HEIGHT = 500;
}
void Quit_CB(Fl_Widget*, void*);
void get_desktop_resolution();
void setup(int argc, char **argv);
void solve_and_display();
void generate_and_display_maze();

int main(int argc, char** argv)
{

	get_desktop_resolution();
	setup(argc, argv);
	return Fl::run();
}


void setup(int argc, char **argv)
{
	//Tegner vindu
	int y = 0;
	Fl_Window *window = new Fl_Window(GLOBALS::SWIDTH, GLOBALS::SHEIGHT);
	window->icon((char*)LoadIcon(fl_display, MAKEINTRESOURCE(101)));
	window->color(FL_WHITE);
	window->labelcolor(FL_BLACK);
	window->label("Maze Solver");
	//Lager menu bar, med exit valg.
	//Kanskje legg til delete eller reset her også?
	Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, y, GLOBALS::SWIDTH, 25);
	menu->add("File/Quit", FL_CTRL + 'q', Quit_CB);
	y += 30;
	
	//Lager overskriften 
	Fl_Box *box =
		new Fl_Box((GLOBALS::SWIDTH - GLOBALS::BOX_WIDTH) / 2, y,
			GLOBALS::BOX_WIDTH, GLOBALS::BOX_HEIGHT,
			"Maze Solver");
	box->box(FL_UP_BOX);
	box->labelfont(FL_BOLD + FL_ITALIC);
	box->labelsize(20);
	box->labeltype(FL_SHADOW_LABEL);

	//Counter for hvor stor labyrint vi skal hente ned.

	y += 70;
	algorithm_chooser = new Fl_Choice
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Velg algoritme");

	algorithm_chooser->add("Prims");
	algorithm_chooser->add("Kruskall");


	y += 70;
	filename_input = new Fl_Input
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "PNG-fil Input");

	y += 70;
	filename_output = new Fl_Input
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "PNG-fil Output");


	y += 70;
	gif_file_output = new Fl_Input
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Gif-fil output");

	y += 70;
	gen_pixel_input = new Fl_Input
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Pixel in generated maze ");

	y += 70;
	status_output = new Fl_Output
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Status");

	y += 70;
	nodes_created_output = new Fl_Value_Output
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Nodes created");

	y += 70;
	distance_output = new Fl_Value_Output
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Distance");

	y += 70;
	time_spent_output = new Fl_Value_Output
	((GLOBALS::SWIDTH - GLOBALS::C_WIDTH) / 2, y, GLOBALS::C_WIDTH, GLOBALS::C_HEIGHT, "Solving/scaling time");
	//
	//Kanskje ha en knapp her for å generere/fetch labyrint
	//
	
	

	
	gen_box = new Fl_Box
	(50, GLOBALS::SHEIGHT / 3, GLOBALS::BILDE2_WIDTH, GLOBALS::BILDE2_HEIGHT);
	//


	solved_box = new Fl_Box
	(GLOBALS::SWIDTH - GLOBALS::SWIDTH/3, GLOBALS::SHEIGHT / 3, GLOBALS::BILDE2_WIDTH, GLOBALS::BILDE2_HEIGHT);

	
	
	//solved_maze->playing();
	
	//Lager generate knapp
	Fl_Button* gen_button = new Fl_Button((GLOBALS::SWIDTH - GLOBALS::BUTTON_WIDTH) / 3
		, GLOBALS::SHEIGHT - 200, GLOBALS::BUTTON_WIDTH, GLOBALS::BUTTON_HEIGHT, "Generate Maze");
	gen_button->type(FL_NORMAL_BUTTON);
	


	//Lager "solve" kanppen.
	Fl_Button* solve_button = new Fl_Button((GLOBALS::SWIDTH - GLOBALS::BUTTON_WIDTH) / 2
		, GLOBALS::SHEIGHT -200, GLOBALS::BUTTON_WIDTH, GLOBALS::BUTTON_HEIGHT, "Solve Maze");
	solve_button->type(FL_NORMAL_BUTTON);



	gen_button->callback((Fl_Callback*)generate_and_display_maze);
	//Legger til funksjon for solve-knappen:
	solve_button->callback((Fl_Callback*)solve_and_display);



	
	window->end();
	window->show(argc, argv);
}


//Henter skjermstørrelse:
void get_desktop_resolution()
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	GLOBALS::SWIDTH = desktop.right;
	GLOBALS::SHEIGHT = desktop.bottom;
}

//Funksjon for "solve"-knapp
void solve_and_display()
{
	status_output->value("Solving Maze...");

	const char* inputvalue = filename_input->value();
	if (*filename_output->value() == '\0' || *gif_file_output->value() == '\0')
	{
		gen_png = new Fl_PNG_Image(inputvalue);

		gen_png_full = gen_png->copy(GLOBALS::BILDE2_HEIGHT, GLOBALS::BILDE2_WIDTH);

		gen_box->image(gen_png_full);
		
		filename_output->value("");
		gif_file_output->value("");
		const char* a = "SOLVED";
		char new_output[100];

		strcpy(new_output, a);
		strcat(new_output, inputvalue);

		filename_output->value(new_output);

		char gif_output[100];
		strcpy(gif_output, "");
		for (int i = 0; i < 100; i++)
		{
			if (new_output[i] == '.')
			{
				strcat(gif_output, ".gif");
				break;
			}
			gif_output[i] = new_output[i];
		}
		gif_file_output->value(gif_output);
	}



	ms = new Maze_Solver(filename_input->value(), filename_output->value(), gif_file_output->value());
	//ms = new Maze_Solver("maze100.png", "output100.png", "finalpath101.gif");
	//ms = new Maze_Solver(filename_input->value(), filename_output->value(), gif_file_output->value());
	//thread t1(ms->solve());
	//Set flag to prevent sleep mode
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
	auto start = chrono::high_resolution_clock::now();
	ready = false;
	ms->solve(& ready);
	
	while (!ready)
	{

	}
	const char* output = filename_output->value();

	status_output->value("Found solution");

	solved_png = new Fl_PNG_Image(output);

	//gen_png_full = gen_png->copy(GLOBALS::BILDE2_HEIGHT, GLOBALS::BILDE2_WIDTH);

	solved_box->image(solved_png);
	//solved_box->image(gen_png);

	Fl::redraw();


	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> diff = end - start;
	cout << "Ferdig!" << endl;
	cout << "maze->solve() tar: " << diff.count() << " sekunder." << endl;
	//Unsets flag to prevent sleep mode.
	SetThreadExecutionState(ES_CONTINUOUS);
	time_spent_output->value(diff.count());
	distance_output->value(ms->distance);
	nodes_created_output->value(ms->nu_of_nodes_created());

	
}

static void generate_and_display_maze()
{
	srand(time(0));
	status_output->value("Generating maze..");
	unsigned int pixels = reinterpret_cast<unsigned int>(gen_pixel_input->value());
	
	
	int alg_int = algorithm_chooser->value();

	switch(alg_int)
	{
	case 0:
		mg = new Rand_Prim_Gen(150);
		break;
	default:
		status_output->value("Error: Choose algorithm");
		return;
	}
	


	//Fl::lock();

	//thread t1(&Maze_Generator::generate, mg, "testgen.png");
	//Fl::unlock();

	//Fl::awake();

	ready = false;
	const char* output = filename_output->value();
	mg->generate(output, &ready);

	//t1.join();
	
	while (!ready)
	{

	}


	gen_png = new Fl_PNG_Image(output);

	gen_png_full = gen_png->copy(GLOBALS::BILDE2_HEIGHT, GLOBALS::BILDE2_WIDTH);

	gen_box->image(gen_png_full);
	//solved_box->image(gen_png);
	

	filename_input->value(output);
	filename_output->value("");

	const char* a = "SOLVED";
	char new_output[100];

	strcpy(new_output, a);
	strcat(new_output, output);

	filename_output->value(new_output);

	char gif_output[100];
	strcpy(gif_output, "");
	for (int i = 0; i < 100; i++)
	{
		if (new_output[i] == '.')
		{
			strcat(gif_output, ".gif");
			break;
		}
		gif_output[i] = new_output[i];
	}
	gif_file_output->value(gif_output);
	


	Fl::redraw();

}

//Funksjon for quit i menu bar
void Quit_CB(Fl_Widget*, void*)
{
	exit(0);
}
