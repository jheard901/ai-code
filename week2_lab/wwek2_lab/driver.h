
#pragma once

#include "puzzle.h"

enum UserMode { PLAYER_CHARACTER, NONPLAYER_CHARACTER };

class TestDriver
{
private:
	enum UserMode playerType;
	
public:
	Puzzle PuzGame;
	int rowSize, colSize, shufTimes;

	void InitGame(int nMode);
	void GenGame(int nRows, int nCols, int nShuffles);	//initializes interface and shuffles board
};

