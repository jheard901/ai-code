
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
	int** iSpace;
	int emptyRow, emptyCol;

	//four temp boards for generating moves
	List* boardList;

	bool bMoveUp;
	bool bMoveDown;
	bool bMoveLeft;
	bool bMoveRight;
public:
	Puzzle();
	static Puzzle &Inst() { static Puzzle puzSolved; return puzSolved; }
	static Puzzle &Self() { static Puzzle puzSelf; return puzSelf; }
	//static List* &Inst2() { static List* puzList; return puzList; }
	~Puzzle();
	void Init(int X, int Y); //create NxN puzzle board
	void Shutdown();
	void Display();
	void Clone();
	void CloneSelf();
	void CloneToBoard(std::vector<std::vector<int>> targetBoard);
	void CloneFromBoard(std::vector<std::vector<int>> srcBoard);
	void CloneFromPuzzle(Puzzle* srcPuzzle);
	bool Solved();	//checks if the puzzle is solved
	void GenMoves();
	bool IsValid(int row, int rowIncrement, int col, int colIncrement); //removed first param (int** &arg, )
	void GetSpace();

	//inline defs
	std::vector<std::vector<int>> GetBoard() { return board; }
	int GetRows() { return rows; }
	int GetCols() { return columns; }
	int GetEmptyRow() { return emptyRow; }
	int GetEmptyCol() { return emptyCol; }
};