#include "Bishop.h"	

Bishop::Bishop() :ChessPiece(){
}

void Bishop::possibleMove(vector<int> bCoords, vector<int> wCoords){ //Get possible move
	//cout << "Pressed Bishop \n";

	// (x,y) -> (x+i*a,x+j*a), i,j = 1/-1
	pair<int, int>* cases = new (std::nothrow) pair<int,int>[4];
	if (!cases)
	{
		cout << "Error allocating Bishop pair list! \n";
		exit(-1);
	}
	cases[0] = make_pair(1, 1);
	cases[1] = make_pair(1, -1);
	cases[2] = make_pair(-1, 1);
	cases[3] = make_pair(-1, -1);

	vector<int> temp;
	int x = this->x;
	int y = this->y;

	for (int i = 0; i < 4; i++) // pair list loop
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

