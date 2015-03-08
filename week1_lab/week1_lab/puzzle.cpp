
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
	
	//init the array; the location with a value of 1 is considered the empty space (i.e. last row, last col)
	InitSpace(rows, columns);
	Puzzle::Self().InitSpace(rows, columns);

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

void Puzzle::InitSpace(int r, int c)
{
	//init the array; the location with a value of 1 is considered the empty space (i.e. last row, last col)
	iSpace = new int*[r];
	for (int i = 0; i < r; i++)
	{
		iSpace[i] = new int[c];
	}

	//fill in the 2d array
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			iSpace[i][j] = 0;
		}
	}
	iSpace[r - 1][c - 1] = 1;
}

void Puzzle::Shutdown()
{
	//delete the dynamic array
	for (int i = 0; i < rows; i++)
	{
		delete[] iSpace[i];
	}
	delete[] iSpace;
}

//GenMoves needs to be ran first before calling display so the location of the space is known
void Puzzle::Display()
{
	currRow = 0;
	currCol = 0;
	const int LIMIT = 10;
	int num;
	std::vector<int>::iterator i;

	//CloneSelf();
	//GetSpace();

	for (unsigned int x = 0; x < board.size(); x++)
	{
		for (unsigned int y = 0; y < board[currRow].size(); y++)
		{
			//check if empty space should be printed
			if (emptyRow == currRow && emptyCol == currCol)
			{
				std::cout << "  " << "  ";
			}

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

void Puzzle::CloneSolution()
{
	for (unsigned int i = 0; i < board.size(); i++)
	{
		Puzzle::Inst().board.push_back(board[i]);
	}
}

//maybe this should be a bool instead?
void Puzzle::CloneSelf()
{
	//ensure board is empty first
	if (!Puzzle::Self().board.empty())
	{
		Puzzle::Self().board.clear();
	}

	Puzzle::Self().rows = rows;
	Puzzle::Self().columns = columns;

	for (unsigned int i = 0; i < board.size(); i++)
	{
		Puzzle::Self().board.push_back(board[i]);
	}

	//gets the empty row and col
	//Puzzle::Self().InitSpace(rows, columns);	//put this into Init() instead
	Puzzle::Self().GetSpace();
	Puzzle::Self().emptyRow = emptyRow;
	Puzzle::Self().emptyCol = emptyCol;
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

//might be missing some important vars, but this looks fine for now
void Puzzle::CloneFromPuzzle(Puzzle* srcPuzzle)
{
	rows = srcPuzzle->GetRows();
	columns = srcPuzzle->GetCols();
	emptyRow = srcPuzzle->GetEmptyRow();
	emptyCol = srcPuzzle->GetEmptyCol();
	CloneFromBoard(srcPuzzle->GetBoard());
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

	for (int i = 0; i < 4; i++)	//use 4 since that is the max number of moves
	{
		boardList->Insert(&Puzzle::Self());
	}
	//boardList->Display();	//debug test
	//std::vector<int> a;
	//a.push_back(0);
	//boardList->head->nPuz->board.push_back(a);


	GetSpace();	//gets the empty row and col	///ignore this -> //moved this functionality into CloneSelf() <- NO! 
	currRow = 0;
	currCol = 0;
	int tempNum;

	//check up *** using board 0
	if (IsValid(emptyRow, -1, emptyCol, 0))
	{
		bMoveUp = true;

		if (iSpace[emptyRow - 1][emptyCol] == 0)
		{
			//choose the first board from the list
			boardList->GoTo(0);

			//present the new possible board state
			tempNum = *boardList->ptr->nPuz->board[emptyRow - 1].begin() + emptyCol;
			boardList->ptr->nPuz->board[emptyRow].insert(boardList->ptr->nPuz->board[emptyRow].begin() + emptyCol, tempNum);
			boardList->ptr->nPuz->board[emptyRow - 1].erase(boardList->ptr->nPuz->board[emptyRow - 1].begin() + emptyCol);

			//update it's space location
			boardList->ptr->nPuz->UpdateSpace(emptyRow - 1, emptyCol);
			boardList->ptr->nPuz->GetSpace();

			//present the new possible board state
			//tempNum = *board[emptyRow - 1].begin() + emptyCol;					//get the value of the location above
			//board[emptyRow].insert(board[emptyRow].begin() + emptyCol, tempNum);	//add that value to the empty space
			//board[emptyRow - 1].erase(board[emptyRow - 1].begin() + emptyCol);	//now erase that value from the original location
		}
	}
	else
	{
		bMoveUp = false;
	}

	//check down *** using board 1
	if (IsValid(emptyRow, 1, emptyCol, 0))
	{
		bMoveDown = true;

		if (iSpace[emptyRow + 1][emptyCol] == 0)
		{
			//choose the second board from the list
			boardList->GoTo(1);

			//present the new possible board state
			tempNum = *boardList->ptr->nPuz->board[emptyRow + 1].begin() + emptyCol;
			boardList->ptr->nPuz->board[emptyRow].insert(boardList->ptr->nPuz->board[emptyRow].begin() + emptyCol, tempNum);
			boardList->ptr->nPuz->board[emptyRow + 1].erase(boardList->ptr->nPuz->board[emptyRow + 1].begin() + emptyCol);

			//update it's space location
			boardList->ptr->nPuz->UpdateSpace(emptyRow + 1, emptyCol);
			boardList->ptr->nPuz->GetSpace();
		}
	}
	else
	{
		bMoveDown = false;
	}

	//check left *** using board 2
	if (IsValid(emptyRow, 0, emptyCol, -1))
	{
		bMoveLeft = true;

		if (iSpace[emptyRow][emptyCol - 1] == 0)
		{
			//choose the third board from the list
			boardList->GoTo(2);

			//present the new possible board state
			tempNum = *boardList->ptr->nPuz->board[emptyRow].begin() + (emptyCol - 1);
			boardList->ptr->nPuz->board[emptyRow].insert(boardList->ptr->nPuz->board[emptyRow].begin() + emptyCol, tempNum);
			boardList->ptr->nPuz->board[emptyRow].erase(boardList->ptr->nPuz->board[emptyRow].begin() + (emptyCol - 1));

			//update it's space location
			boardList->ptr->nPuz->UpdateSpace(emptyRow, emptyCol - 1);
			boardList->ptr->nPuz->GetSpace();
		}
	}
	else
	{
		bMoveLeft = false;
	}

	//check right *** using board 3
	if (IsValid(emptyRow, 0, emptyCol, 1))
	{
		bMoveRight = true;

		if (iSpace[emptyRow][emptyCol + 1] == 0)
		{
			//choose the fourth board from the list
			boardList->GoTo(3);

			//present the new possible board state
			tempNum = *boardList->ptr->nPuz->board[emptyRow].begin() + (emptyCol + 1);
			boardList->ptr->nPuz->board[emptyRow].insert(boardList->ptr->nPuz->board[emptyRow].begin() + emptyCol, tempNum);
			boardList->ptr->nPuz->board[emptyRow].erase(boardList->ptr->nPuz->board[emptyRow].begin() + (emptyCol + 1));

			//update it's space location
			boardList->ptr->nPuz->UpdateSpace(emptyRow, emptyCol + 1);
			boardList->ptr->nPuz->GetSpace();
		}
	}
	else
	{
		bMoveRight = false;
	}
	
	//display all moves to user
	//boardList->Display();

	//display valid moves to user
	if (bMoveUp)	{ boardList->Display(0); }
	if (bMoveDown)	{ boardList->Display(1); }
	if (bMoveLeft)	{ boardList->Display(2); }
	if (bMoveRight)	{ boardList->Display(3); }
}

bool Puzzle::IsValid(int row, int rowIncrement, int col, int colIncrement)
{

	int mr = rows - 1;				//max number of rows
	int mc = columns - 1;			//max number of cols
	int nrp = row + rowIncrement;	//new proposed position of space in row
	int ncp = col + colIncrement;	//new proposed position of space in col

	//this means the new position of space (via col) does not fall within the bounds of the array itself (i.e. bottom)
	if (nrp > mr)
	{
		return false;
	}

	//this means the new position of space (via col) does not fall within the bounds of the array itself (i.e. top)
	if (nrp < 0)
	{
		return false;
	}
	
	//this means the new position of space (via row) does not fall within the bounds of the array on right side
	if (ncp > mc)
	{
		return false;
	}

	//this means the new position of space (via row) does not fall within the bounds of the array on left side
	if (ncp < 0)
	{
		return false;
	}

	return true;
}

//this is only used after calling CloneSelf() (i.e. in GenMoves() )
void Puzzle::GetSpace()
{
	currRow = 0;
	currCol = 0;

	//check each row to see which one has a space (or rather, has N - 1 columns in a single row)
	//rows
	for (unsigned int i = 0; i < board.size(); i++)	//changed to use just board, took out Puzzle::Inst() from board.size(); hope there were no dependencies on this
	{
		//cols
		for (unsigned int j = 0; j < board[i].size(); j++)
		{
			currCol++;
		}

		if (currCol != columns)
		{
			//this row is the one with the space
			emptyRow = currRow;

			for (int z = 0; z < sizeof(iSpace[emptyRow]); z++)
			{
				if (iSpace[emptyRow][z] == 1)
				{
					//this col is the one with the space
					emptyCol = z;
					return;
				}
			}
		}
		else
		{
			currCol = 0;
			currRow++;
		}
	}
}

void Puzzle::UpdateSpace(int newRow, int newCol)
{
	//make all values 0
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			iSpace[i][j] = 0;
		}
	}

	//set new location of space
	iSpace[newRow][newCol] = 1;
}

