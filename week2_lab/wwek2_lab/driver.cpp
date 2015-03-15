
#include "driver.h"
#include <iostream>
#include <stdlib.h> //srand(), rand()
#include <time.h> //time()

#define NEW_LINE(x) { for(int i = 0; i < x; i++) { std::cout << std::endl; } }	//quick macro for new lines
const int LEN = 2;

TestDriver::TestDriver()
{
	//player settings
	rowSize = 0;
	colSize = 0;
	shufTimes = 0;

	//ai settings
	aiRowSize = 3;
	aiColSize = 3;
	aiShufTimes = 20;

	//seed random number generator once
	srand(unsigned(time(NULL)));
}

TestDriver::~TestDriver()
{

}

//Starts up the game for a player; 0 = person playing, 1 = AI playing
void TestDriver::InitGame(int nMode)
{
	if (nMode == 0)
	{
		playerType = PLAYER_CHARACTER;
	}
	else if (nMode == 1)
	{
		playerType = NONPLAYER_CHARACTER;
	}
	else if (nMode == 2)
	{
		playerType = DEV_CHARACTER;
	}
	else { return; }

	///////////////////////////////////////////
	//START GAME HERE
	///////////////////////////////////////////	P.S - should be using a switch/case probs
	
	//Human Player
	if (playerType == PLAYER_CHARACTER)
	{
		std::cout << "row size: ";
		std::cin >> rowSize;
		std::cout << "col size: ";
		std::cin >> colSize;
		std::cout << "shuffle times: ";
		std::cin >> shufTimes;
		GenGame(rowSize, colSize, shufTimes);
		NEW_LINE(LEN);

		//The Game Loop
		while (!PuzGame.Solved())
		{
			PuzGame.GetUserInput();
		}
		
		std::cout << "\n\nYou win!";
		NEW_LINE(LEN);
		PuzGame.DisplayStats();
		return;
	}

	//AI Player
	else if (playerType == NONPLAYER_CHARACTER)
	{

	}

	//For Debugging
	else if (playerType == DEV_CHARACTER)
	{
		GenGame(4, 6, 2);	//input values directly

		//The Game Loop
		while (!PuzGame.Solved())
		{
			PuzGame.GetUserInput();
		}

		std::cout << "\n\nYou win!";
		NEW_LINE(LEN);
		PuzGame.DisplayStats();
		return;
	}
	else { return; }

}

//generates NxN puzzle and shuffles it
void TestDriver::GenGame(int nRows, int nCols, int nShuffles)
{
	PuzGame.Init(nRows, nCols);
	PuzGame.CloneSolution();	//creates a copy of solution
	std::cout << "\nAssemble Like This:\n";
	PuzGame.GetSpace();
	PuzGame.Display();

	PuzGame.Shuffle(nShuffles);

	std::cout << "\n\nCurrent Board State:\n";
	PuzGame.Display();
	//std::cout << "\n\n\nPrevious Board States:\n";
	//prevBoardList->Display();
}

