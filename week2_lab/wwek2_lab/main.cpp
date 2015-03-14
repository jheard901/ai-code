// Javier Heard
// GSP480
// 3-12-15
//
// The N-Puzzle

//#include "puzzle.h"
#include "driver.h"
#include <iostream>
#include <stdlib.h> //srand(), rand()
#include <time.h> //time()


#define NEW_LINE(x) { for(int i = 0; i < x; i++) { std::cout << std::endl; } }	//quick macro for new lines
const int LEN = 2;

int main()
{
	//seed random number generator once
	srand(unsigned(time(NULL)));
	TestDriver testGame;


	///////////////////////////////////////////
	//Start test driver here
	///////////////////////////////////////////
	testGame.InitGame(0);

	//int rowSize, colSize, shufTimes;
	//Puzzle PuzGame;

	//uncomment this after debugging
	//std::cout << "row size: ";
	//std::cin >> rowSize;
	//std::cout << "col size: ";
	//std::cin >> colSize;
	//std::cout << "shuffle times: ";
	//std::cin >> shufTimes;
	//PuzGame.GenGame(rowSize, colSize, shufTimes);
	//PuzGame.GenGame(4, 4, 20);

	//NEW_LINE(LEN);
	//while (!PuzGame.Solved())
	//{
	//	PuzGame.GetUserInput();
	//}
	//
	//std::cout << "\n\nYou win!";
	//NEW_LINE(LEN);
	//PuzGame.DisplayStats();
	NEW_LINE(LEN);
	system("pause");

	return 0;
}
