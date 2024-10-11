#include "Knight.h"
#include "Board.h"
Knight::Knight() :ChessPiece() {
}



void Knight::possibleMove(vector<int> bCoords, vector<int> wCoords){ //Get possible move
	//cout << "Pressed Knight! \n";
	
	//8 case: 
	pair<int, int>* pairList = new (std::nothrow) pair<int,int>[8];
	if (!pairList)
	{
		cout << "Error allocating in Knight! \n";
		exit(-1);
	}

	pairList[0] = make_pair(2, 1);
	pairList[1] = make_pair(2, -1);
	pairList[2] = make_pair(-2, 1);
	pairList[3] = make_pair(-2, -1);
	pairList[4] = make_pair(1, 2);
	pairList[5] = make_pair(1, -2);
	pairList[6] = make_pair(-1, 2);
	pairList[7] = make_pair(-1, -2);


	for (int i = 0; i <= 7; i++)
	{
		int x2 = this->x + pairList[i].first;
		int y2 = this->y + pairList[i].second;
		
		//outside board:
		if (this->isOutside(x2, y2)) continue;
		
		if (this->isBlack)
		{
			
				if (this->inside(x2, y2, wCoords)) this->kill.push_back(x2 * 8 + y2);
				else if (this->inside(x2, y2, bCoords)) this->guard.push_back(x2 * 8 + y2);
				else this->move.push_back(x2 * 8 + y2);
			
		}
		else {
			if (this->inside(x2, y2, bCoords)) this->kill.push_back(x2 * 8 + y2);
			else if (this->inside(x2, y2, wCoords)) this->guard.push_back(x2 * 8 + y2);
			else this->move.push_back(x2 * 8 + y2);
		}
	}
	
	//dealloc
	delete[]pairList;
	pairList = nullptr;
		
}

//void Knight::clearUnableMove(vector<int> temp) {
//	for (int i = 0; i < this->move.size(); i++)
//	{
//		vector<int>::iterator it = std::find(temp.begin(), temp.end(), this->move[i]);
//		if (it == temp.end()) continue;//safe move
//		cout << "Found unsafe move \n";
//
//		this->move.erase(this->move.begin() + i);//delete unsafe move
//		i--;
//	}
//
//	for (int i = 0; i < this->kill.size(); i++)
//	{
//		vector<int>::iterator it = std::find(temp.begin(), temp.end(), this->kill[i]);
//		if (it == temp.end()) continue;//safe move
//		cout << "Found unsafe move \n";
//		this->kill.erase(this->kill.begin() + i);//delete unsafe move
//		i--;
//	}
//}