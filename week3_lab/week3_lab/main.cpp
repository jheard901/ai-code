// Javier Heard
// GSP480
// 3-18-15
//
// The NxM-Puzzle

//#include "puzzle.h"
#include "driver.h"
#include <iostream>

const int PLAYER = 0;
const int AI = 1;	//ai values are set up from within TestDriver constructor
const int AI2 = 2;
const int DEV = 3;

int main()
{
	TestDriver testGame;	//Note, RNG is seeded in the TestDriver constructor

	///////////////////////////////////////////
	//Start test driver here
	///////////////////////////////////////////
	//std::cout << "Select user mode: \n";
	//std::cout << "0 = PLAYER\n";
	//std::cout << "1 = AI\n";
	//std::cout << "2 = AI2\n";
	//std::cout << "3 = DEV\n";
	//int input;
	//std::cin >> input;

	testGame.InitGame(AI2);	//accepts values 0 - 3
	std::cout << "\n\n";
	system("pause");
	return 0;
}

