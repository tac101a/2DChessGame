#pragma once
#include "ChessPiece.h"
#include <vector>

class Pawn : public ChessPiece
{
public:
	Pawn();
	bool reachedFinal();

	//-----VIRTUAL METHOD-------
	void possibleMove(vector<int> bCoords, vector<int> wCoords); //Get possible move

	//bool checkCrossKillSpecial(vector<int> bCoords, vector<int> wCoords);
	void setNumMoveFirst(int);
	int getNumMoveFirst();
private:
	void crossKill(vector<int> bCoords, vector<int> wCoords); //Calculate possible kill & guard
	
	int numMoveFirst;
};
