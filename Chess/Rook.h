#pragma once
#include "ChessPiece.h"

class Rook : public ChessPiece
{
public:
	Rook();
	void possibleMove(vector<int> bCoords, vector<int> wCoords); //Get possible move
private:
	
};
