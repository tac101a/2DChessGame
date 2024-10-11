#pragma once
#include "ChessPiece.h"

class King: public ChessPiece
{
public:
	King();
	void possibleMove(vector<int> bCoords, vector<int> wCoords); //Get possible move
	
private:
	
};

