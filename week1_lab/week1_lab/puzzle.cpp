
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
	//Puzzle::Space()
	
	//init the array; the location with a value of 1 is considered the empty space (i.e. last row, last col)
	iSpace = new int*[rows];
	for (int i = 0; i < rows; i++)
	{
		iSpace[i] = new int[columns];
	}
	//fill in the 2d array
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			iSpace[i][j] = 0;
		}
	}
	iSpace[rows - 1][columns - 1] = 1;

	//iColSpace = new int[columns];
	//for (int i = 0; i < columns; i++)
	//{
	//	iColSpace[i] = 0;
	//}
	
	//iRowSpace = rows - 1;
	//iColSpace[columns - 1] = 1;

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
	//delete the dynamic array
	for (int i = 0; i < rows; i++)
	{
		delete[] iSpace[i];
	}
	delete[] iSpace;
	//delete[] Puzzle::Space();		//when I delete this, the program does not immediately exit - it lags for about 4-5 seconds; should this be done a different way?
	//delete []iColSpace;
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
	
	//int spaceCol = GetSpace();
	//int spaceRow = iRowSpace;

	GetSpace();
	currRow = 0;
	currCol = 0;
	//check up
	if (iSpace[emptyRow - 1][emptyCol] == 0)
	{
		//present the new possible board state

	}
	//check down
	if (iSpace[emptyRow + 1][emptyCol] == 0)
	{

	}
	//check left
	if (iSpace[emptyRow][emptyCol - 1] == 0)
	{

	}
	//check right
	if (iSpace[emptyRow][emptyCol + 1] == 0)
	{

	}
	
	//create 2d array
	//int** b = new int*[rows];
	//for (int i = 0; i < rows; i++)
	//{
	//	b[i] = new int[columns];
	//}

	//init the 2d array
	//for (int i = 0; i < rows; i++)
	//{
	//	for (int j = 0; j < columns; j++)
	//	{
	//		b[i][j] = 0;
	//	}
	//}

	//create the space in 2d array
	//b[spaceRow][spaceCol] = 1;

	//check if can move up
	//if (b[spaceRow - 1][spaceCol] = 0)
	//{
	//	//present the new possible state
	//
	//}
	////for (int x = 0; x < Puzzle::Inst().board[spaceRow].size(); x++)
	////{
	////}
	//
	////check if can move down
	//if (b[spaceRow + 1][spaceCol] = 0)
	//{
	//
	//}
	//
	////check if can move left
	//if (b[spaceRow][spaceCol - 1] = 0)
	//{
	//
	//}
	//
	////check if can move right
	//if (b[spaceRow][spaceCol + 1] = 0)
	//{
	//
	//}
	//
	////delete 2d array
	//for (int i = 0; i < rows; i++)
	//{
	//	delete[] b[i];
	//}
	//delete[] b;
	
}

std::vector<std::vector<int>> Puzzle::GetBoard()
{
	return board;
}

//this is only used after calling CloneSelf() (i.e. in GenMoves() )
void Puzzle::GetSpace()
{
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
			//const int check = 1;	//check which spot the space is in
			//
			//for (int i = 0; i < sizeof(iColSpace); i++)
			//{
			//	if (check == iColSpace[i])
			//	{
			//		iRowSpace = currRow;
			//		return currCol;
			//	}
			//}

			emptyRow = currRow;

			for (int z = 0; z < sizeof(iSpace[emptyRow]); z++)
			{
				if (iSpace[emptyRow][z] == 1)
				{
					emptyCol = z;
				}
			}
			//pCol = currCol;
			//this doesn't really matter since the space is known from start
		}
		else
		{
			currRow++;
		}
	}
}