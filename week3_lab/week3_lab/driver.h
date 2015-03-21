
#pragma once

#include "puzzle.h"

enum UserMode { PLAYER_CHARACTER, NONPLAYER_CHARACTER, NONPLAYER_CHARACTER2, NONPLAYER_CHARACTER3, DEV_CHARACTER };

class TestDriver
{
private:
	enum UserMode playerType;
public:
	Puzzle PuzGame;
	int rowSize, colSize, shufTimes;
	int aiRowSize, aiColSize, aiShufTimes;

	TestDriver();
	~TestDriver();
	void InitGame(int nMode);
	void GenGame(int nRows, int nCols, int nShuffles);	//initializes interface and shuffles board
	void GetUserInput();
	void SetBoardState();	//gets input on what size NxN board a user would like to use
};

