
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

	//init the container for board states (or technically Puzzles)
	boardList = new List;

	//init the container for storing previous board states
	prevBoardList = new List;

	//init the array; the location with a value of 1 is considered the empty space (i.e. last row, last col)
	InitSpace(rows, columns);
	Puzzle::Self().InitSpace(rows, columns);
	//Puzzle::Prev().InitSpace(rows, columns);

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

//GenMoves needs to be ran first before calling display so the location of the space is known; or rather GetSpace()
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

//copies most of this object's values into the Puzzle::Self() static object
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

//this seems like it may be the source of the problem for numbers not displaying properly; I probably need to change the code where I present new board states
void Puzzle::GenMoves()
{
	//updating the space is done in CloneSelf() as well
	CloneSelf();

	//make sure the board list is clear
	boardList->Reset();

	for (int i = 0; i < 4; i++)	//use 4 since that is the max number of moves
	{
		boardList->Insert(&Puzzle::Self());
	}


	GetSpace();	//gets the empty row and col
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
			tempNum = *boardList->ptr->nPuz->board[emptyRow - 1].begin() + emptyCol;											//get the value of the location above
			boardList->ptr->nPuz->board[emptyRow].insert(boardList->ptr->nPuz->board[emptyRow].begin() + emptyCol, tempNum);	//add that value to the empty space
			boardList->ptr->nPuz->board[emptyRow - 1].erase(boardList->ptr->nPuz->board[emptyRow - 1].begin() + emptyCol);		//now erase that value from the original location

			//update it's space location
			boardList->ptr->nPuz->UpdateSpace(emptyRow - 1, emptyCol);
			boardList->ptr->nPuz->GetSpace();
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

}

//only call this after running GenMoves()
void Puzzle::DisplayGenMoves()
{
	if (bMoveUp)	{ std::cout << "\nState: 0\n"; boardList->Display(0); }
	if (bMoveDown)	{ std::cout << "\nState: 1\n"; boardList->Display(1); }
	if (bMoveLeft)	{ std::cout << "\nState: 2\n"; boardList->Display(2); }
	if (bMoveRight)	{ std::cout << "\nState: 3\n"; boardList->Display(3); }
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
	for (unsigned int i = 0; i < board.size(); i++)
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

//generates NxN puzzle and shuffles it
void Puzzle::GenGame(int nRows, int nCols, int nShuffles)
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

void Puzzle::GetUserInput()
{
	int userPick;

	GenMoves();
	prevBoardList->Insert(&Puzzle::Self());	//store our old state down
	DisplayGenMoves();
	std::cout << "\nPick new state: ";
	std::cin >> userPick;
	while (!PickNewState(userPick))
	{
		std::cout << "\nerror - invalid state entered";
		std::cout << "\nPick new state: ";
		std::cin >> userPick;
	}
	std::cout << "\n\nCurrent Board State:\n";
	Display();
}

//randomly selects a board state from boardlist; must run GenMoves before this
void Puzzle::RandNewState()
{
	//later, I should probably add a condition to check if the boardlist is empty
	int randNum;
	bool bValidMove = false;
	int sameValue;
	int maxSameValue = board.size();

	while (!bValidMove)
	{
		randNum = GetRandomInt(0, 3);	//must be from range 0 - 3 to choose a valid board state
		sameValue = 0;					//reset sameValue

		if (randNum == 0)
		{
			//can move up?
			if (bMoveUp)
			{
				//count shared values by first board and old board state
				boardList->GoTo(0);
				for (unsigned int i = 0; i < boardList->ptr->nPuz->board.size(); i++)	//rows
				{
					for (unsigned int j = 0; j < boardList->ptr->nPuz->board[i].size(); j++)	//cols
					{
						if (*boardList->ptr->nPuz->board[i].begin() + j == *Puzzle::Self().board[i].begin() + j)
						{
							sameValue++;
						}
					}
				}

				//is not same as last board state? //this method needs to be done better later on
				if (sameValue != maxSameValue)
				{
					bValidMove = true;
				}
			}
		}
		else if (randNum == 1)
		{
			//can move down?
			if (bMoveDown)
			{
				//count shared values by second board and old board state
				boardList->GoTo(1);
				for (unsigned int i = 0; i < boardList->ptr->nPuz->board.size(); i++)	//rows
				{
					for (unsigned int j = 0; j < boardList->ptr->nPuz->board[i].size(); j++)	//cols
					{
						if (*boardList->ptr->nPuz->board[i].begin() + j == *Puzzle::Self().board[i].begin() + j)
						{
							sameValue++;
						}
					}
				}

				//is not same as last board state? //this method needs to be done better later on
				if (sameValue != maxSameValue)
				{
					bValidMove = true;
				}
			}
		}
		else if (randNum == 2)
		{
			//can move left?
			if (bMoveLeft)
			{
				//count shared values by third board and old board state
				boardList->GoTo(2);
				for (unsigned int i = 0; i < boardList->ptr->nPuz->board.size(); i++)	//rows
				{
					for (unsigned int j = 0; j < boardList->ptr->nPuz->board[i].size(); j++)	//cols
					{
						if (*boardList->ptr->nPuz->board[i].begin() + j == *Puzzle::Self().board[i].begin() + j)
						{
							sameValue++;
						}
					}
				}

				//is not same as last board state? //this method needs to be done better later on
				if (sameValue != maxSameValue)
				{
					bValidMove = true;
				}
			}
		}
		else if (randNum == 3)
		{
			//can move right?
			if (bMoveRight)
			{
				//count shared values by fourth board and old board state
				boardList->GoTo(3);
				for (unsigned int i = 0; i < boardList->ptr->nPuz->board.size(); i++)	//rows
				{
					for (unsigned int j = 0; j < boardList->ptr->nPuz->board[i].size(); j++)	//cols
					{
						if (*boardList->ptr->nPuz->board[i].begin() + j == *Puzzle::Self().board[i].begin() + j)
						{
							sameValue++;
						}
					}
				}

				//is not same as last board state? //this method needs to be done better later on
				if (sameValue != maxSameValue)
				{
					bValidMove = true;
				}
			}
		}
		else
		{
			//this else statement should never happen though
		}
	}

	//set our current board to the new board's state
	//also, these functions are essentially all you need whenever you want to update the current board to a new board state
	CloneFromPuzzle(boardList->ptr->nPuz);
	UpdateSpace(emptyRow, emptyCol);
	GetSpace();
}

//must be between 0 - 3 as per max num of board states
bool Puzzle::PickNewState(int nState)
{
	//later, I should probably add a condition to check if the boardlist is empty
	int userNum = nState;
	bool bValidMove = false;
	int sameValue;
	int maxSameValue = board.size();

	while (!bValidMove)
	{
		sameValue = 0;	//reset sameValue

		if (userNum == 0)
		{
			//can move up?
			if (bMoveUp)
			{
				bValidMove = true;
			}
			else
			{
				return false;
			}
		}
		else if (userNum == 1)
		{
			//can move down?
			if (bMoveDown)
			{
				bValidMove = true;
			}
			else
			{
				return false;
			}
		}
		else if (userNum == 2)
		{
			//can move left?
			if (bMoveLeft)
			{
				bValidMove = true;
			}
			else
			{
				return false;
			}
		}
		else if (userNum == 3)
		{
			//can move right?
			if (bMoveRight)
			{
				bValidMove = true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			//this else statement should never happen though
		}
	}

	//set our current board to the new board's state
	//also, these functions are essentially all you need whenever you want to update the current board to a new board state
	CloneFromPuzzle(boardList->ptr->nPuz);
	UpdateSpace(emptyRow, emptyCol);
	GetSpace();
	return true;
}

void Puzzle::DisplayStats()
{
	std::cout << "\nBoard State History:";
	prevBoardList->Display();
	std::cout << "\nMoves Taken: " << prevBoardList->Length();

}

