#include "Queen.h"

Queen::Queen() :ChessPiece() {

}

void Queen::possibleMove(vector<int> bCoords, vector<int> wCoords){ //Get possible move
	//cout << "Pressed Queen! \n";

	pair<int, int>* cases = new (std::nothrow) pair<int, int>[8];
	if (!cases)
	{
		cout << "Error allocating Queen pair list! \n";
		exit(-1);
	}
	cases[0] = make_pair(1, 1);
	cases[1] = make_pair(1, -1);
	cases[2] = make_pair(-1, 1);
	cases[3] = make_pair(-1, -1);
	cases[4] = make_pair(1, 0);
	cases[5] = make_pair(-1, 0);
	cases[6] = make_pair(0, 1);
	cases[7] = make_pair(0, -1);

	int x = this->x;
	int y = this->y;
	
	for (int i = 0; i < 8; i++) // pair list loop
	{
		bool flag = false;
		for (int a = 1; a < 8; a++)
		{
			int x2 = x + cases[i].first * a;
			int y2 = y + cases[i].second * a;

			this->moveCal(bCoords, wCoords, x2, y2, flag);
		}
	}

	delete[] cases;
	cases = nullptr;


}
