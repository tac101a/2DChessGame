#include "Pawn.h"

Pawn::Pawn() :ChessPiece() {
}

bool Pawn::reachedFinal() {
	if (this->x == 0 || this->x == 7)
	{
		return true;
	}
	return false;
}

void Pawn::possibleMove(vector<int> bCoords, vector<int> wCoords) {
	int x = this->x; //toa do doc
	int y = this->y; //toa do ngang

	//cout << "Pressed Pawn " << x << " and " << y << "\n";
	//Mo ban:
	if (this->isBlack && x == 1)
	{
		//Move 1 - 2
		for (int i = 0; i < 2; i++)
		{
			if (this->isOutside(x + i + 1, y)) break;
			if (this->inside(x + i + 1, y, bCoords)) break;
			if (this->inside(x + i + 1, y, wCoords)) break;
			this->move.push_back(8 * (x + i + 1) + y);
		}
	}

	if (!this->isBlack && x == 6)
	{
		//Move 1 - 2
		for (int i = 0; i < 2; i++)
		{
			if (this->isOutside(x - i - 1, y)) break;
			if (this->inside(x - i - 1, y, bCoords)) break;
			if (this->inside(x - i - 1, y, wCoords)) break;
			this->move.push_back(8 * (x - i - 1) + y);
		}
	}

	//Trung & hau ban:
	//Move 1
	if (this->isBlack && x != 1)
	{
		if (!this->isOutside(x + 1, y) && !this->inside(x + 1, y, bCoords) && !this->inside(x + 1, y, wCoords)) 
			this->move.push_back(8 * (x + 1) + y);
	}
	//White:
	if (!this->isBlack && x != 6)
	{
		if (!this->isOutside(x - 1, y) && !this->inside(x - 1, y, bCoords) && !this->inside(x - 1, y, wCoords))
			this->move.push_back(8 * (x - 1) + y);
	}

	//Cross kill
	this->crossKill(bCoords, wCoords);
	return;
}

//bool Pawn::checkCrossKillSpecial(vector<int> bCoords, vector<int> wCoords) {
//	if (this->isBlack)
//	{
//		//x+1, y+-1
//		//Outside the board
//		if (this->isOutside(x + 1, y)) return;
//		//pushback kill:
//		if (!this->isOutside(x + 1, y + 1))
//		{
//			if (this->inside(x + 1, y + 1, wCoords)) return;
//			else if ((x*8)+y+1<64)
//			{
//
//			}
//			this->guard.push_back((x + 1) * 8 + y + 1);
//		}
//		if (!this->isOutside(x + 1, y - 1))
//		{
//			if (this->inside(x + 1, y - 1, wCoords)) this->kill.push_back((x + 1) * 8 + y - 1);
//			this->guard.push_back((x + 1) * 8 + y - 1);
//		}
//	}
//	else {
//		//x-1, y+-1
//		//Outside the board
//		if (this->isOutside(x - 1, y)) return;
//
//		if (!this->isOutside(x - 1, y + 1)) {
//			if (this->inside(x - 1, y + 1, bCoords)) this->kill.push_back((x - 1) * 8 + y + 1);
//			this->guard.push_back((x - 1) * 8 + y + 1);
//		}
//
//		if (!this->isOutside(x - 1, y - 1)) {
//			if (this->inside(x - 1, y - 1, bCoords)) this->kill.push_back((x - 1) * 8 + y - 1);
//			this->guard.push_back((x - 1) * 8 + y - 1);
//		}
//	}
//}
void Pawn::crossKill(vector<int> bCoords, vector<int> wCoords) {
	// x, y
	if (this->isBlack)
	{
		//x+1, y+-1
		//Outside the board
		if (this->isOutside(x + 1, y)) return;
		//pushback kill:
		if (!this->isOutside(x + 1, y + 1))
		{
			if (this->inside(x + 1, y + 1, wCoords)) this->kill.push_back((x + 1) * 8 + y + 1);
			this->guard.push_back((x + 1) * 8 + y + 1);
		}
		if (!this->isOutside(x + 1, y - 1))
		{
			if (this->inside(x + 1, y - 1, wCoords)) this->kill.push_back((x + 1) * 8 + y - 1);
			this->guard.push_back((x + 1) * 8 + y - 1);
		}
	}
	else {
		//x-1, y+-1
		//Outside the board
		if (this->isOutside(x - 1, y)) return;

		if (!this->isOutside(x - 1, y + 1)) {
			if (this->inside(x - 1, y + 1, bCoords)) this->kill.push_back((x - 1) * 8 + y + 1);
			this->guard.push_back((x - 1) * 8 + y + 1);
		}

		if (!this->isOutside(x - 1, y - 1)) {
			if (this->inside(x - 1, y - 1, bCoords)) this->kill.push_back((x - 1) * 8 + y - 1);
			this->guard.push_back((x - 1) * 8 + y - 1);
		}
	}
}


void Pawn::setNumMoveFirst(int x) {
	this->numMoveFirst = x;
}
int Pawn::getNumMoveFirst() {
	return this->numMoveFirst;
}