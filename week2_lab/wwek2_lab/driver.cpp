
#include "driver.h"
#include <iostream>

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
	else { return; }

	//start the game with chosen player type
	if (playerType == PLAYER_CHARACTER)
	{
		std::cout << "row size: ";
		std::cin >> rowSize;
		std::cout << "col size: ";
		std::cin >> colSize;
		std::cout << "shuffle times: ";
		std::cin >> shufTimes;
		PuzGame.GenGame(rowSize, colSize, shufTimes);
	}
	else if (playerType == NONPLAYER_CHARACTER)
	{

	}
	else
	{

	}

}

//generates NxN puzzle and shuffles it
void TestDriver::GenGame(int nRows, int nCols, int nShuffles)
{
	Init(nRows, nCols);
	CloneSolution();	//creates a copy of solution
	std::cout << "\nAssemble Like This:\n";
	GetSpace();	//caution for errors
	Display();

	//int nShuffles = 20;
	for (int i = 0; i < nShuffles; i++)
	{
		GenMoves();
		//prevBoardList->Insert(&Puzzle::Self());	//store our old state down
		//DisplayGenMoves();
		RandNewState();
	}
	std::cout << "\n\nCurrent Board State:\n";
	Display();
	//std::cout << "\n\n\nPrevious Board States:\n";
	//prevBoardList->Display();
}