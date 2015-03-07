
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

	//four temp boards for generating moves
	List* boardList;
public:
	Puzzle();
	static Puzzle &Inst() { static Puzzle puzSolved; return puzSolved; }
	static Puzzle &Self() { static Puzzle puzSelf; return puzSelf; }
	//static List* &Inst2() { static List* puzList; return puzList; }
	~Puzzle();
	void Init(int X, int Y); //create NxN puzzle board
	void Display();
	void Clone();
	void CloneSelf();
	void CloneToBoard(std::vector<std::vector<int>> targetBoard);
	void CloneFromBoard(std::vector<std::vector<int>> srcBoard);
	bool Solved();	//checks if the puzzle is solved
	void GenMoves();
	std::vector<std::vector<int>> GetBoard();
};