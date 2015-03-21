
#include "driver.h"
#include <iostream>
#include <stdlib.h> //srand(), rand()
#include <time.h> //time()

#define NEW_LINE(x) { for(int i = 0; i < x; i++) { std::cout << std::endl; } }	//quick macro for new lines
const int LEN = 2;

TestDriver::TestDriver()
{
	//default player settings
	rowSize = 2;
	colSize = 2;
	shufTimes = 6;

	//default ai settings
	aiRowSize = 3;
	aiColSize = 3;
	aiShufTimes = 20;
}

TestDriver::~TestDriver()
{

}

//Starts up the game for a player; 0 = person playing, 1 = AI playing
void TestDriver::InitGame(int nMode)
{
	//seed random number generator once
	//srand(unsigned(time(NULL)));

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
		playerType = NONPLAYER_CHARACTER2;
	}
	else if (nMode == 3)
	{
		playerType = DEV_CHARACTER;
	}
	else { return; }

	///////////////////////////////////////////
	//START GAME HERE
	///////////////////////////////////////////	P.S - should be using a switch/case probs
	
	//lets user set initial board size and shuffle times
	//SetBoardState();

	//Human Player
	if (playerType == PLAYER_CHARACTER)
	{
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

	//AI Player (incredibly slow compared to AI with heuristics)
	else if (playerType == NONPLAYER_CHARACTER)
	{
		GenGame(aiRowSize, aiColSize, aiShufTimes);

		if (PuzGame.SolveAttempt(&PuzGame))
		{
			std::cout << "\n\nYou win!";
			NEW_LINE(LEN);
			PuzGame.DisplayStats();
			NEW_LINE(LEN);
			std::cout << "Your Board:\n";
			PuzGame.Display();
			return;
		}
	}

	//AI Player with heuristics
	else if (playerType == NONPLAYER_CHARACTER2)
	{
		GenGame(aiRowSize, aiColSize, aiShufTimes);

		if (PuzGame.SolveAttemptH(&PuzGame))
		{
			std::cout << "\n\nYou win!";
			NEW_LINE(LEN);
			PuzGame.DisplayStats();
			NEW_LINE(LEN);
			std::cout << "Your Board:\n";
			PuzGame.Display();
			return;
		}
	}

	//For Debugging
	else if (playerType == DEV_CHARACTER)
	{
		GenGame(3, 3, 2);	//input values directly

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

void TestDriver::SetBoardState()
{
	switch (playerType)
	{
	case PLAYER_CHARACTER:
		std::cout << "row size: ";
		std::cin >> rowSize;
		std::cout << "col size: ";
		std::cin >> colSize;
		std::cout << "shuffle times: ";
		std::cin >> shufTimes;
		break;
	case NONPLAYER_CHARACTER:
		std::cout << "row size: ";
		std::cin >> aiRowSize;
		std::cout << "col size: ";
		std::cin >> aiColSize;
		std::cout << "shuffle times: ";
		std::cin >> aiShufTimes;
		break;
	case NONPLAYER_CHARACTER2:
		std::cout << "row size: ";
		std::cin >> aiRowSize;
		std::cout << "col size: ";
		std::cin >> aiColSize;
		std::cout << "shuffle times: ";
		std::cin >> aiShufTimes;
		break;
	case DEV_CHARACTER:
		break;
	}
}