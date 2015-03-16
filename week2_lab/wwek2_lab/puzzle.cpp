
#include "puzzle.h"
#include "list.h"
#include "math.h"
#include <iostream>

Puzzle::Puzzle()
	:	movesTaken(0),
		depth(0)
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

	total = (X * Y);	//total number of pieces on board. 0 = the empty space.

	//init the container for board states (or technically Puzzles)
	boardList = new List;
	
	//init the container for storing previous board states
	prevBoardList = new List;

	//init list for use with IDFS
	validMoves = new List;
	theSolPath = new List;
	solutionPath = new List;
	recursePath = new List;

	//init board
	if (!board.empty())
	{
		board.clear();
	}

	//fill in board
	for (int x = 0; x < total; x++)
	{
		//make the last number inserted = to 0
		if (x == total - 1)
		{
			a.push_back(0);
			currCol++;
		}
		else
		{
			a.push_back(x + 1);		//use +1 so we start from 1 instead of 0
			currCol++;
		}
		
		//check if we should start on a new row
		if (currCol == columns)
		{
			board.push_back(a);
			a.clear();
			currCol = 0;
			currRow++;
		}
	}
}

void Puzzle::Shutdown()
{
	
}

//GenMoves needs to be ran first before calling display so the location of the space is known; or rather GetSpace()
void Puzzle::Display()
{
	currRow = 0;
	currCol = 0;
	const int LIMIT = 10;
	int num;

	for (unsigned int x = 0; x < board.size(); x++)
	{
		for (unsigned int y = 0; y < board[currRow].size(); y++)
		{
			//this is purely for visual appearance to make single digit numbers, line up with double digit numbers
			num = board[currRow].at(y);
			if (num < LIMIT)
			{
				//is the num our empty space?
				if (num == 0)
				{
					std::cout << "  " << "  ";
					currCol++;
				}
				else
				{
					std::cout << "0" << board[currRow].at(y) << "  ";
					currCol++;
				}
			}
			else
			{
				std::cout << board[currRow].at(y) << "  ";
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

	//gets solution puzzle
	//Puzzle::Self().CloneSolution();	//this might cause errors
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
	int tempNum;

	//rows
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)
		{
			tempNum = board[i].at(j);

			if (tempNum != Puzzle::Inst().board[i].at(j))
			{
				return false;
			}
		}
	}

	return true;
}

bool Puzzle::Solved(std::vector<std::vector<int>> Board)
{
	int tempNum;

	//rows
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)
		{
			tempNum = Board[i].at(j);

			if (tempNum != Puzzle::Inst().board[i].at(j))
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

	GetSpace();
	currRow = 0;
	currCol = 0;
	int tempNum;

	//check up | using board 0
	if (IsValid(emptyRow, -1, emptyCol, 0))
	{
		bMoveUp = true;

		//choose the first board from the list
		boardList->GoTo(0);

		//present the new possible board state
		//tempNum = *boardList->ptr->nPuz->board[emptyRow - 1].begin() + emptyCol;											//get the value of the location above
		//boardList->ptr->nPuz->board[emptyRow].insert(boardList->ptr->nPuz->board[emptyRow].begin() + emptyCol, tempNum);	//add that value to the empty space
		//boardList->ptr->nPuz->board[emptyRow - 1].erase(boardList->ptr->nPuz->board[emptyRow - 1].begin() + emptyCol);		//now erase that value from the original location

		//present the new possible board state
		tempNum = boardList->ptr->nPuz->board[emptyRow - 1].at(emptyCol);	//get the value of the location above
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol) = tempNum;				//change the empty space (0) to be that value
		boardList->ptr->nPuz->board[emptyRow - 1].at(emptyCol) = 0;					//make the location above the empty space (0)

		//update it's space location
		boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		bMoveUp = false;
	}

	//check down | using board 1
	if (IsValid(emptyRow, 1, emptyCol, 0))
	{
		bMoveDown = true;

		//choose the second board from the list
		boardList->GoTo(1);

		//present the new possible board state
		tempNum = boardList->ptr->nPuz->board[emptyRow + 1].at(emptyCol);
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol) = tempNum;
		boardList->ptr->nPuz->board[emptyRow + 1].at(emptyCol) = 0;

		//update it's space location
		boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		bMoveDown = false;
	}

	//check left | using board 2
	if (IsValid(emptyRow, 0, emptyCol, -1))
	{
		bMoveLeft = true;

		//choose the third board from the list
		boardList->GoTo(2);

		//present the new possible board state
		tempNum = boardList->ptr->nPuz->board[emptyRow].at(emptyCol - 1);
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol) = tempNum;
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol - 1) = 0;

		//update it's space location
		boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		bMoveLeft = false;
	}

	//check right | using board 3
	if (IsValid(emptyRow, 0, emptyCol, 1))
	{
		bMoveRight = true;

		//choose the fourth board from the list
		boardList->GoTo(3);

		//present the new possible board state
		tempNum = boardList->ptr->nPuz->board[emptyRow].at(emptyCol + 1);
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol) = tempNum;
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol + 1) = 0;

		//update it's space location
		boardList->ptr->nPuz->GetSpace();
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

//fills in emptyRow and emptyCol based off which row and col has the value 0 in it
void Puzzle::GetSpace()
{
	currRow = 0;
	currCol = 0;
	int tempNum;

	//rows
	for (unsigned int i = 0; i < board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < board[i].size(); j++)
		{
			tempNum = board[i].at(j);
			if (tempNum == 0)
			{
				emptyRow = currRow;
				emptyCol = currCol;

				return;
			}
			else
			{
				currCol++;
			}
		}
		currCol = 0;
		currRow++;
	}
}

void Puzzle::Shuffle(int nTimes)
{
	for (int i = 0; i < nTimes; i++)
	{
		GenMoves();
		//prevBoardList->Insert(&Puzzle::Self());	//store our old state down
		//DisplayGenMoves();
		RandNewState();
	}
}

//should only be used by test driver
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
	movesTaken++;
}

//randomly selects a board state from boardlist; must run GenMoves before this
void Puzzle::RandNewState()
{
	int randNum;
	bool bValidMove = false;
	int sameValue;
	int maxSameValue = total;

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
						if (boardList->ptr->nPuz->board[i].at(j) == Puzzle::Self().board[i].at(j))
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
						if (boardList->ptr->nPuz->board[i].at(j) == Puzzle::Self().board[i].at(j))
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
						if (boardList->ptr->nPuz->board[i].at(j) == Puzzle::Self().board[i].at(j))
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
						//if (*boardList->ptr->nPuz->board[i].begin() + j == *Puzzle::Self().board[i].begin() + j)
						if (boardList->ptr->nPuz->board[i].at(j) == Puzzle::Self().board[i].at(j))
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
	GetSpace();
}

//must be between 0 - 3 as per max num of board states
bool Puzzle::PickNewState(int nState)
{
	//later, I should probably add a condition to check if the boardlist is empty
	int userNum = nState;
	bool bValidMove = false;

	while (!bValidMove)
	{

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
	boardList->GoTo(userNum);
	CloneFromPuzzle(boardList->ptr->nPuz);
	GetSpace();
	return true;
}

void Puzzle::DisplayStats()
{
	std::cout << "\nBoard State History:";
	prevBoardList->Display();
	std::cout << "\nMoves Taken: " << movesTaken;
}

bool Puzzle::SolveAttempt(Puzzle* aPuz)
{
	CloneSelf();

	while (!aPuz->Solved())
	{
		solutionPath->Reset();

		theSolPath->Copy(IDFS(depth, aPuz));
		if (theSolPath->tail != nullptr)
		{
			CloneFromBoard(theSolPath->tail->nPuz->board);
		}
		depth++;
	}
	return true;
}

//must be run after gen moves
void Puzzle::StoreMoves(Puzzle* puz)
{
	int randNum;
	int sum = 0;
	std::vector<int> valid;

	//clear list if not already empty
	if (validMoves->Length() > 0)
	{
		validMoves->Reset();
	}

	//check the valid moves
	if (bMoveUp) { valid.push_back(0); sum++; }
	if (bMoveDown) { valid.push_back(1); sum++; }
	if (bMoveLeft) { valid.push_back(2); sum++; }
	if (bMoveRight) { valid.push_back(3); sum++; }
	
	
	//can only use sum > 1 here
	while (sum > 1)
	{
		randNum = GetRandomInt(0, float(sum - 1));
		boardList->GoTo(valid.at(randNum));
		CloneFromPuzzle(boardList->ptr->nPuz);
		GetSpace();
		validMoves->Insert(puz);

		valid.erase(valid.begin() + randNum);
		sum--;
	}
	//then input the final move
	if (sum > 0)
	{
		boardList->GoTo(valid.at(0));
		CloneFromPuzzle(boardList->ptr->nPuz);
		GetSpace();
		validMoves->Insert(puz);

		valid.erase(valid.begin());
		sum--;
	}

}

List* Puzzle::IDFS(int Depth, Puzzle* iPuz)
{
	movesTaken++;
	CloneSelf();
	solutionPath->Insert(iPuz);

	if (Solved(iPuz->board))	//this might work
	{
		return solutionPath;
	}

	//start over if reached max depth
	if (Depth == 0)
	{
		solutionPath->DeleteLast();
		return NULL;	//might need changes to accomodate for the list type
	}

	//otherwise, keep searching for solution
	GenMoves();
	StoreMoves(iPuz);
	for (int i = 0; i < validMoves->Length(); i++)
	{
		validMoves->GoTo(i);
		recursePath->Copy(IDFS(Depth - 1, validMoves->ptr->nPuz));
		//recursePath->Copy(IDFS(Depth - 1, validMoves->ptr->nPuz));	//original
		if (recursePath->tail != nullptr)
		{
			return recursePath;	//return if we found a solution on one of the alternate path
		}
	}
	solutionPath->DeleteLast();
	return NULL;
}

//original for reference
//List* Puzzle::IDFS(int Depth, Puzzle* iPuz)
//{
//	movesTaken++;
//	iPuz->CloneSelf();
//	iPuz->solutionPath->Insert(&iPuz->Puzzle::Self());
//
//	if (Solved(iPuz->board))	//this might work
//	{
//		return iPuz->solutionPath;
//	}
//
//	//start over if reached max depth
//	if (Depth == 0)
//	{
//		iPuz->solutionPath->DeleteLast();
//		return NULL;	//might need changes to accomodate for the list type
//	}
//
//	//otherwise, keep searching for solution
//	iPuz->GenMoves();
//	iPuz->StoreMoves();
//	for (int i = 0; i < iPuz->validMoves->Length(); i++)
//	{
//		iPuz->validMoves->GoTo(i);
//		iPuz->recursePath->Copy(IDFS(Depth - 1, iPuz->validMoves->ptr->nPuz));
//		if (iPuz->recursePath->tail != nullptr)
//		{
//			return iPuz->recursePath;	//return if we found a solution on one of the alternate path
//		}
//	}
//	iPuz->solutionPath->DeleteLast();
//	return NULL;
//}

