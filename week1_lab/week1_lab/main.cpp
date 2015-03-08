//The 15-Puzzle Problem
//
//Javier Heard
//3-6-15
//GSP480

#include "math.h"
#include "puzzle.h"
#include <iostream>


#define NEW_LINE(x) { for(int i = 0; i < x; i++) { std::cout << std::endl; } }	//quick macro for new lines
const int LEN = 2;

int main()
{
	srand(unsigned(time(NULL))); //seed random number generator once

	Puzzle PuzGame;
	PuzGame.Init(3, 3);
	PuzGame.CloneSolution();	//creates a copy of solution
	//PuzGame.Display();
	PuzGame.GenMoves();
	NEW_LINE(LEN);
	PuzGame.Display();
	

	if (PuzGame.Solved())
	{
		std::cout << "\n\nYou win!";
	}
	NEW_LINE(LEN);
	system("pause");

	return 0;
}


