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
const int DEV = 2;
int main()
{
	TestDriver testGame;	//Note, RNG is seeded in the TestDriver constructor
	//int input;

	///////////////////////////////////////////
	//Start test driver here
	///////////////////////////////////////////
	//std::cout << "Select user mode: \n";
	//std::cout << "0 = PLAYER\n";
	//std::cout << "1 = AI\n";
	//std::cout << "2 = DEV\n";
	//std::cin >> input;

	testGame.InitGame(AI);	//accepts values 0 - 2
	std::cout << "\n\n";
	system("pause");
	return 0;
}
