
#include "puzzle.h"
#include "list.h"
#include "math.h"
#include <iostream>

Puzzle::Puzzle()
{

}

Puzzle::~Puzzle()
{
	Shutdown();
}

void Puzzle::Init(int X, int Y)
{
	rows = X;
	columns = Y;
	currRow = 0;
	currCol = 0;
	std::vector<int> a;

	int total = (X * Y) - 1;	//use -1 so it abides by number of pieces allotted in the NxN puzzle

	//create the space to keep track of it on the board; only need to know which column the space is in though
	//iSpace = new int*[rows];
	Puzzle::Space() = new int[columns];
	//for (int i = 0; i < rows; i++)
	//{
	//	//iSpace[i] = new int[columns];
	//	Puzzle::Space()[i] = new int[columns];
	//}

	//the location with a value of 1 is considered the empty space
	Puzzle::Space()[columns - 1] = 1;
	//iSpace[rows][columns] = 1;
	//Puzzle::Space()[rows][columns] = 1;

	//init board
	if (board.empty())
	{
		for (int x = 0; x < total; x++)
		{
			a.push_back(x + 1);		//use +1 so we start from 1 instead of 0
			currCol++;

			//check if we should start on a new row
			if (currCol == columns)
			{
				board.push_back(a);
				a.clear();
				currCol = 0;
				currRow++;
			}
		}
		//push back the final row into the board; needed since we will always exit early on the last row which is will be 1 short of being the same length as a completed row
		board.push_back(a);
		a.clear();
	}
	else
	{
		board.clear();
	}
}

void Puzzle::Shutdown()
{
	//delete dynamic array
	//for (int i = 0; i < rows; i++)
	//{
	//	//delete [] iSpace[i];
	//	delete [] Puzzle::Space()[i];
	//}
	
	//delete the Space;
	delete[] Puzzle::Space();
}

void Puzzle::Display()
{
	currRow = 0;
	currCol = 0;
	const int LIMIT = 10;
	int num;
	std::vector<int>::iterator i;

	for (unsigned int x = 0; x < board.size(); x++)
	{
		for (unsigned int y = 0; y < board[currRow].size(); y++)
		{
			//this is purely for visual appearance to make single digit numbers, line up with double digit numbers
			i = board[currRow].begin() + y;
			num = *i;
			if (num < LIMIT)
			{
				std::cout << "0" << *board[currRow].begin() + y << "  ";
				currCol++;
			}
			else
			{
				std::cout << *board[currRow].begin() + y << "  ";
				currCol++;
			}
		}
		
		//start on a new row
		std::cout << "\n";
		currCol = 0;
		currRow++;
	}
}

void Puzzle::Clone()
{
	for (unsigned int i = 0; i < board.size(); i++)
	{
		Puzzle::Inst().board.push_back(board[i]);
		//for (unsigned int j = 0; j < board[i].size(); j++)
		//{
		//}
	}
}

void Puzzle::CloneSelf()
{
	for (unsigned int i = 0; i < board.size(); i++)
	{
		Puzzle::Self().board.push_back(board[i]);
	}
}

void Puzzle::CloneToBoard(std::vector<std::vector<int>> targetBoard)
{
	for (unsigned int i = 0; i < board.size(); i++)
	{
		targetBoard.push_back(board[i]);
	}
}

void Puzzle::CloneFromBoard(std::vector<std::vector<int>> srcBoard)
{
	//ensure board is empty first
	if (!board.empty())
	{
		board.clear();
	}

	for (unsigned int i = 0; i < srcBoard.size(); i++)
	{
		board.push_back(srcBoard[i]);
	}
}

bool Puzzle::Solved()
{
	//rows
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)
		{
			if (*board[i].begin() + j != *Puzzle::Inst().board[i].begin() + j)
			{
				return false;
			}
		}
	}
	
	return true;
}

void Puzzle::GenMoves()
{
	boardList = new List;
	CloneSelf();

	for (int i = 0; i < 4; i++)
	{
		boardList->Insert(&Puzzle::Self());
	}
	//boardList->Display();	//debug test
	//std::vector<int> a;
	//a.push_back(0);
	//boardList->head->nPuz->board.push_back(a);

	currRow = 0;
	currCol = 0;

	//check each row to see which one has a space (or rather, has N - 1 columns in a single row)
	//rows
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)
		{
			currCol++;
		}

		if (currCol != columns)
		{
			//this row is deemed as the one with the space
			//the question now is, how do we determine where our space is placed at in the row?
		}
		else
		{
			currRow++;
		}
	}
}

std::vector<std::vector<int>> Puzzle::GetBoard()
{
	return board;
}
