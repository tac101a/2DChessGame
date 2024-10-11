#pragma once
#include "ChessPiece.h"

class Knight: public ChessPiece
{
public:
	Knight();
	void possibleMove(vector<int> bCoords, vector<int> wCoords); //Get possible move
	//void clearUnableMove(vector<int> temp);

private:
};

