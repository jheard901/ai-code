
#pragma once

#include <vector>

class List;

class Puzzle
{
private:
	int rows;
	int columns;
	int currRow;
	int currCol;
	std::vector<std::vector<int>> board;
	int emptyRow, emptyCol;
	int total;	//total num pieces in play
	int movesTaken;
	int depth;
	int costLimit, costIncrement;

	//four temp boards for generating moves
	List* boardList;
	//stores a list of all previous board states
	List* prevBoardList;
	//stores list of all possible paths that can be taken from a board state
	List* validMoves;
	//stores the solution path of IDFS algorithm
	List* theSolPath;
	//stores the resulting solution path of IDFS during runtime
	List* solutionPath;
	//stores the alternate paths to traverse in IDFS
	List* recursePath;

	bool bMoveUp;
	bool bMoveDown;
	bool bMoveLeft;
	bool bMoveRight;
public:
	Puzzle();
	static Puzzle &Inst() { static Puzzle puzSolved; return puzSolved; }
	static Puzzle &Self() { static Puzzle puzSelf; return puzSelf; }
	static Puzzle &Prev() { static Puzzle puzPrev; return puzPrev; }
	//static List* &Inst2() { static List* puzList; return puzList; }
	~Puzzle();
	void Init(int X, int Y); //create NxN puzzle board
	void Shutdown();
	void Display();
	void CloneSolution();
	void CloneSelf();
	void CloneToBoard(std::vector<std::vector<int>> targetBoard);
	void CloneFromBoard(std::vector<std::vector<int>> srcBoard);
	void CloneFromPuzzle(Puzzle* srcPuzzle);
	bool Solved();	//checks if the puzzle is solved
	bool Solved(std::vector<std::vector<int>> Board);	//checks if a board matches the solution
	void GenMoves();
	void GenMoves(Puzzle* puz);
	void DisplayGenMoves();	//only call this after running GenMoves()
	bool IsValid(int row, int rowIncrement, int col, int colIncrement);
	void GetSpace();
	void Shuffle(int nTimes);	//shuffles board
	void GetUserInput();
	void RandNewState();	//randomly selects a board state
	bool PickNewState(int nState);	//allows picking a board state
	void DisplayStats();	//only use after game is completed

	bool SolveAttempt(Puzzle* aPuz);	//used only by driver
	bool SolveAttemptH(Puzzle* aPuz);	//uses a heuristic
	void StoreMoves(Puzzle* puz);
	List* IDFS(int Depth, Puzzle* iPuz);
	List* DFA(int CostLimit, int Depth, Puzzle* iPuz);	//uses a cost function to limit it
	int DistFromGoal(int value, int cr, int cc);	//returns how far away a value is from its solution state; cr = currRow, cc = currCol
	int HCost(std::vector<std::vector<int>> Board);	//returns cost of board relative to the solution state (via Manhattan)
	
	//uses the simple heuristic
	bool SolveAttemptHS(Puzzle* aPuz);
	int HCostSimple(std::vector<std::vector<int>> Board);
	List* DFAS(int CostLimit, int Depth, Puzzle* iPuz);

	//inline defs
	std::vector<std::vector<int>> GetBoard() { return board; }
	int GetRows() { return rows; }
	int GetCols() { return columns; }
	int GetEmptyRow() { return emptyRow; }
	int GetEmptyCol() { return emptyCol; }
};

