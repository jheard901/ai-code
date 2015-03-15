// Javier Heard
// GSP480
// 3-12-15
//
// The NxM-Puzzle

//#include "puzzle.h"
#include "driver.h"
#include <iostream>






const int PLAYER = 0;
const int AI = 1;	//ai values are set up from within TestDriver constructor
const int DEBUG = 2;
int main()
{
	TestDriver testGame;	//Note, RNG is seeded in the TestDriver constructor

	///////////////////////////////////////////
	//Start test driver here
	///////////////////////////////////////////
	testGame.InitGame(DEBUG);	//accepts values 0 - 2
	std::cout << "\n\n";
	system("pause");
	return 0;
}
