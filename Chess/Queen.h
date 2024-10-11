#pragma once
#include "ChessPiece.h"

class Queen:public ChessPiece
{
public:
	Queen();
	void possibleMove(vector<int> bCoords, vector<int> wCoords); //Get possible move

private:

};
