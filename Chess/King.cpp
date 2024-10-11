#include "King.h"

King::King(): ChessPiece() {

}

void King::possibleMove(vector<int> bCoords, vector<int> wCoords){ //Get possible move
	//cout << "Pressed King! \n";
	int x = this->x;
	int y = this->y;

	pair<int, int>* cases = new pair<int, int>[8];
	if (!cases)
	{
		cout << "Error allocating King's pair list \n";
		exit(-1);
	}

	cases[0] = make_pair(0, 1);
	cases[1] = make_pair(0, -1);
	cases[2] = make_pair(1, 0);
	cases[3] = make_pair(-1, 0);
	cases[4] = make_pair(1, 1);
	cases[5] = make_pair(1, -1);
	cases[6] = make_pair(-1, 1);
	cases[7] = make_pair(-1, -1);

	for (int i = 0; i < 8; i++) // pair list loop
	{
		bool flag = false;
		int x2 = x + cases[i].first;
		int y2 = y + cases[i].second;

		this->moveCal(bCoords, wCoords, x2, y2, flag);
	}

	

	delete[] cases;
	cases = nullptr;

}


