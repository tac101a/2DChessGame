#include"Rook.h"

Rook::Rook() : ChessPiece(){
}


void Rook::possibleMove(vector<int> bCoords, vector<int> wCoords){ //Get possible move
	//cout << "Pressed Rook! \n";
	int x = this->x;
	int y = this->y;

	//4 cases:
	pair<int, int>* cases = new	(std::nothrow) pair<int, int>[4];
	if (!cases)
	{
		cout << "Error allocating Rook pair list\n";
		exit(-1);
	}

	cases[0] = make_pair(1, 0);
	cases[1] = make_pair(-1, 0);
	cases[2] = make_pair(0, 1);
	cases[3] = make_pair(0, -1);

	for (int i = 0; i < 4; i++)
	{
		bool flag = false;
		for (int j = 1; j < 8; j++)
		{

			int x2 = x + cases[i].first * j;
			int y2 = y + cases[i].second * j;

			this->moveCal(bCoords, wCoords, x2, y2, flag);
		}
	}
	delete[] cases;
	cases = nullptr;
}


