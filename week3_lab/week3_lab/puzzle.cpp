
#include "puzzle.h"
#include "list.h"
#include "math.h"
#include <iostream>
#include <cmath>	//abs()

Puzzle::Puzzle()
	:	movesTaken(0),
		depth(0)
{
	//need these created for each new Puzzle for IDFS to work properly
	boardList = new List;
	validMoves = new List;
	recursePath = new List;
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
	
	//init the container for storing previous board states (for players)
	prevBoardList = new List;

	//init list for use with IDFS (for AI)
	theSolPath = new List;
	solutionPath = new List;

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
	//Puzzle::Self().emptyRow = emptyRow;
	//Puzzle::Self().emptyCol = emptyCol;

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
		tempNum = boardList->ptr->nPuz->board[emptyRow - 1].at(emptyCol);	//get the value of the location above
		boardList->ptr->nPuz->board[emptyRow].at(emptyCol) = tempNum;		//change the empty space (0) to be that value
		boardList->ptr->nPuz->board[emptyRow - 1].at(emptyCol) = 0;			//make the location above the empty space (0)

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

void Puzzle::GenMoves(Puzzle* puz)
{
	//make sure the board list is clear
	puz->boardList->Reset();

	for (int i = 0; i < 4; i++)	//use 4 since that is the max number of moves
	{
		puz->boardList->Insert(puz);
	}

	puz->GetSpace();
	int tempNum;

	//check up | using board 0
	if (IsValid(puz->emptyRow, -1, puz->emptyCol, 0))
	{
		puz->bMoveUp = true;

		//choose the first board from the list
		puz->boardList->GoTo(0);

		//present the new possible board state
		tempNum = puz->boardList->ptr->nPuz->board[puz->emptyRow - 1].at(puz->emptyCol);	//get the value of the location above
		puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol) = tempNum;		//change the empty space (0) to be that value
		puz->boardList->ptr->nPuz->board[puz->emptyRow - 1].at(puz->emptyCol) = 0;			//make the location above the empty space (0)

		//update it's space location
		puz->boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		puz->bMoveUp = false;
	}

	//check down | using board 1
	if (IsValid(puz->emptyRow, 1, puz->emptyCol, 0))
	{
		puz->bMoveDown = true;

		//choose the second board from the list
		puz->boardList->GoTo(1);

		//present the new possible board state
		tempNum = puz->boardList->ptr->nPuz->board[puz->emptyRow + 1].at(puz->emptyCol);
		puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol) = tempNum;
		puz->boardList->ptr->nPuz->board[puz->emptyRow + 1].at(puz->emptyCol) = 0;

		//update it's space location
		puz->boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		puz->bMoveDown = false;
	}

	//check left | using board 2
	if (IsValid(puz->emptyRow, 0, puz->emptyCol, -1))
	{
		puz->bMoveLeft = true;

		//choose the third board from the list
		puz->boardList->GoTo(2);

		//present the new possible board state
		tempNum = puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol - 1);
		puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol) = tempNum;
		puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol - 1) = 0;

		//update it's space location
		puz->boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		puz->bMoveLeft = false;
	}

	//check right | using board 3
	if (IsValid(puz->emptyRow, 0, puz->emptyCol, 1))
	{
		puz->bMoveRight = true;

		//choose the fourth board from the list
		puz->boardList->GoTo(3);

		//present the new possible board state
		tempNum = puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol + 1);
		puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol) = tempNum;
		puz->boardList->ptr->nPuz->board[puz->emptyRow].at(puz->emptyCol + 1) = 0;

		//update it's space location
		puz->boardList->ptr->nPuz->GetSpace();
	}
	else
	{
		puz->bMoveRight = false;
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
void Puzzle::RandNewState()	//later, I could update this to target a specific board, rather than the original puzzle board only
{
	bool bValidMove = false;
	int randNum;
	int sameValue;
	int maxSameValue = total;
	int sum = 0;
	std::vector<int> valid;

	//check the valid moves
	if (bMoveUp) { valid.push_back(0); sum++; }
	if (bMoveDown) { valid.push_back(1); sum++; }
	if (bMoveLeft) { valid.push_back(2); sum++; }
	if (bMoveRight) { valid.push_back(3); sum++; }

	while (!bValidMove)
	{
		sameValue = 0;

		//can only use sum > 1 here
		if (sum > 1)
		{
			randNum = GetRandomInt(0, float(sum - 1));
			boardList->GoTo(valid.at(randNum));
			valid.erase(valid.begin() + randNum);
			sum--;
		}
		//use the final move
		else if (sum == 1)
		{
			boardList->GoTo(valid.at(0));
			valid.erase(valid.begin());
			sum--;
		}

		//check the previous board is not empty first
		if(!Puzzle::Prev().board.empty())
		{
			//count shared values by a new random board state and prev board state
			for (unsigned int i = 0; i < boardList->ptr->nPuz->board.size(); i++)	//rows
			{
				for (unsigned int j = 0; j < boardList->ptr->nPuz->board[i].size(); j++)	//cols
				{
					if (boardList->ptr->nPuz->board[i].at(j) == Puzzle::Prev().board[i].at(j))
					{
						sameValue++;
					}
				}
			}
			}

		//is not same as last board state?
		if (sameValue != maxSameValue)
		{
			bValidMove = true;
		}
	}
	//store the prev board (i.e. current board since its about to be changed) to compare it to the next board state
	Puzzle::Prev().CloneFromPuzzle(&Puzzle::Self());

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

//attempt to solve using a heuristic
bool Puzzle::SolveAttemptH(Puzzle* aPuz)
{
	//set initial cost limit
	depth = 0;
	costLimit = HCost(aPuz->board);
	costIncrement = aPuz->rows * aPuz->columns;	//experiment with different values here

	//the game loop
	while (!aPuz->Solved())
	{
		solutionPath->Reset();
		std::cout << "\nCost Limit: " << costLimit << std::endl;	//visual debug

		theSolPath->Copy(DFA(costLimit, depth, aPuz));
		if (theSolPath->tail != nullptr)
		{
			CloneFromBoard(theSolPath->tail->nPuz->board);
		}
		costLimit += costIncrement;
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
	if (puz->validMoves->Length() > 0)
	{
		puz->validMoves->Reset();
	}

	//check the valid moves
	if (puz->bMoveUp) { valid.push_back(0); sum++; }
	if (puz->bMoveDown) { valid.push_back(1); sum++; }
	if (puz->bMoveLeft) { valid.push_back(2); sum++; }
	if (puz->bMoveRight) { valid.push_back(3); sum++; }
	
	
	//can only use sum > 1 here
	while (sum > 1)
	{
		randNum = GetRandomInt(0, float(sum - 1));
		puz->boardList->GoTo(valid.at(randNum));
		puz->validMoves->Insert(puz->boardList->ptr->nPuz);

		valid.erase(valid.begin() + randNum);
		sum--;
	}
	//then input the final move
	if (sum > 0)
	{
		puz->boardList->GoTo(valid.at(0));
		puz->validMoves->Insert(puz->boardList->ptr->nPuz);

		valid.erase(valid.begin());
		sum--;
	}

}

List* Puzzle::IDFS(int Depth, Puzzle* iPuz)
{
	movesTaken++;
	solutionPath->Insert(iPuz);

	//visual debug help
	std::cout << "\ndepth = " << Depth << "\n";
	iPuz->Display();

	if (Solved(iPuz->board))	//this might work
	{
		return solutionPath;
	}

	//start over if reached max depth
	if (Depth == 0)
	{
		solutionPath->DeleteLast();
		return NULL;
	}

	//otherwise, keep searching for solution
	GenMoves(iPuz);
	StoreMoves(iPuz);
	for (int i = 0; i < iPuz->validMoves->Length(); i++)
	{
		iPuz->validMoves->GoTo(i);
		iPuz->recursePath->Copy(IDFS(Depth - 1, iPuz->validMoves->ptr->nPuz));
		if (iPuz->recursePath->tail != nullptr)
		{
			return iPuz->recursePath;	//return if we found a solution on one of the alternate path
		}
	}
	solutionPath->DeleteLast();
	return NULL;
}

List* Puzzle::DFA(int CostLimit, int Depth, Puzzle* iPuz)
{
	movesTaken++;
	solutionPath->Insert(iPuz);

	//visual debug help
	std::cout << "\ndepth = " << Depth << "\n";
	iPuz->Display();

	if (Solved(iPuz->board))
	{
		return solutionPath;
	}

	//keep searching for solution
	GenMoves(iPuz);
	StoreMoves(iPuz);
	for (int i = 0; i < iPuz->validMoves->Length(); i++)
	{
		iPuz->validMoves->GoTo(i);	//possibly could improve this by going through each move, get their heuristic cost, compare to find the lowest, then start with that board

		//only explore boards with good heuristic values
		if (HCost(iPuz->validMoves->ptr->nPuz->board) + Depth <= CostLimit)
		{
			iPuz->recursePath->Copy(DFA(CostLimit, Depth + 1, iPuz->validMoves->ptr->nPuz));
			if (iPuz->recursePath->tail != nullptr)
			{
				return iPuz->recursePath;	//return if we found a solution on one of the alternate path
			}
		}
	}
	solutionPath->DeleteLast();
	return NULL;
}

//returns distance a value is from its solution state
int Puzzle::DistFromGoal(int value, int cr, int cc)
{
	bool bFoundGoal = false;
	int goalRow = 0;
	int goalCol = 0;

	//find what row and col the value should be in
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)	//rows
	{
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)	//cols
		{
			if (value == Puzzle::Inst().board[i].at(j))
			{
				bFoundGoal = true;
				break;
			}
			goalCol++;
		}
		if (bFoundGoal)
		{
			break;
		}
		goalRow++;
		goalCol = 0;
	}

	int dr = goalRow - cr;	//dist from currrow to goalrow
	int dc = goalCol - cc;	//dist from currcol to goalcow
	int dt = std::abs(dr) + std::abs(dc);	//total dist

	return dt;
}

//returns cost of board relative to the solution state
int Puzzle::HCost(std::vector<std::vector<int>> Board)
{
	int tempNum;
	int pathCost = 0;
	currRow = 0;
	currCol = 0;

	//rows
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)
		{
			tempNum = Board[i].at(j);

			if (tempNum != Puzzle::Inst().board[i].at(j))
			{
				pathCost += DistFromGoal(tempNum, currRow, currCol);
			}
			currCol++;
		}
		currRow++;
		currCol = 0;
	}

	return pathCost;
}

int Puzzle::HCostSimple(std::vector<std::vector<int>> Board)
{
	int pathCost = 0;
	currRow = 0;
	currCol = 0;

	//rows
	for (unsigned int i = 0; i < Puzzle::Inst().board.size(); i++)
	{
		//cols
		for (unsigned int j = 0; j < Puzzle::Inst().board[i].size(); j++)
		{
			if (Board[i].at(j) != Puzzle::Inst().board[i].at(j))
			{
				pathCost++;
			}
			currCol++;
		}
		currRow++;
		currCol = 0;
	}

	return pathCost;
}

bool Puzzle::SolveAttemptHS(Puzzle* aPuz)
{
	//set initial cost limit
	depth = 0;
	costLimit = HCostSimple(aPuz->board);
	costIncrement = aPuz->rows * aPuz->columns;	//experiment with different values here

	//the game loop
	while (!aPuz->Solved())
	{
		solutionPath->Reset();
		std::cout << "\nCost Limit: " << costLimit << std::endl;	//visual debug

		theSolPath->Copy(DFAS(costLimit, depth, aPuz));
		if (theSolPath->tail != nullptr)
		{
			CloneFromBoard(theSolPath->tail->nPuz->board);
		}
		costLimit += costIncrement;
	}
	return true;
}

List* Puzzle::DFAS(int CostLimit, int Depth, Puzzle* iPuz)
{
	movesTaken++;
	solutionPath->Insert(iPuz);

	//visual debug help
	std::cout << "\ndepth = " << Depth << "\n";
	iPuz->Display();

	if (Solved(iPuz->board))
	{
		return solutionPath;
	}

	//keep searching for solution
	GenMoves(iPuz);
	StoreMoves(iPuz);
	for (int i = 0; i < iPuz->validMoves->Length(); i++)
	{
		iPuz->validMoves->GoTo(i);	//possibly could improve this by going through each move, get their heuristic cost, compare to find the lowest, then start with that board

		//only explore boards with good heuristic values
		if (HCostSimple(iPuz->validMoves->ptr->nPuz->board) + Depth <= CostLimit)
		{
			iPuz->recursePath->Copy(DFAS(CostLimit, Depth + 1, iPuz->validMoves->ptr->nPuz));
			if (iPuz->recursePath->tail != nullptr)
			{
				return iPuz->recursePath;	//return if we found a solution on one of the alternate path
			}
		}
	}
	solutionPath->DeleteLast();
	return NULL;
}