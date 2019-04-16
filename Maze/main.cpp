#include "Maze_Solver.h"
#include "Rand_Prim_Gen.h"

//#include "../../Maze_GUI/Maze_GUI/Maze_GUI.h"

void solve();
void generate();

int main()
{
	cout << "MAZE\n\n[G]enerate\n[S]olve\n[Q]uit" << endl;

	char ch;

	cin >> ch;

	switch (ch)
	{
	case 'g':
		generate();
		break;
	case 's':
		solve();
		break;
	}
	char c;
	cout << "Ferdig!";
	cin >> c;
}

void generate()
{
	cout << "Enter outputfilename + '.png'" << endl;
	string filename;
	cin >> filename;
	srand(time(0));
	Maze_Generator* mg = new Rand_Prim_Gen(100);
	mg->generate(filename.c_str());
}

void solve()
{	
	string infile;
	string outPNG;
	string outGIF;

	cout << "Enter input filename: ";
	cin >> infile;
	
	outPNG = "SOLVED" + infile;
	outGIF = outPNG.substr(0, outPNG.size() - 4) + ".gif";




	Maze_Solver* ms = new Maze_Solver(infile.c_str(), outPNG.c_str(), outGIF.c_str());
	ms->solve();
}




