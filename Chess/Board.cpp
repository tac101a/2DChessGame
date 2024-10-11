#include "Board.h"
#include<sstream>
#include<fstream>
#include<string>
#include<thread>
#include<chrono>

#include"Promote.h"
//-------CONSTRUCTOR & GRAPHIC SETTING---------
Board::Board(sf::Color c1, sf::Color c2) {
	this->sound=new Sound();
	this->square = new (std::nothrow) ChessCell[64]; //64 squares in a chessboard

	this->cellPressed = -1;
	if (!this->square)
	{
		throw "Failed allocating in class Board! \n";
		exit(-1);
	}
	this->isBlackTurn = false;
	display(c1, c2);
}

Board::~Board() {
	delete[] this->square;
	this->square = nullptr;
	
	this->blackCoords.clear();
	this->whiteCoords.clear();
}



void Board::saveChess(stringstream &s) {
	fstream f;
	if (this->numMove != 0) {
		f.open("../saveChess.txt", ios_base::out | ios_base::app | ios_base::binary);
		if (f.is_open()) {
			f << _strdup(s.str().c_str()) << endl;
		}
		f.close();
	}
	else {
		f.open("../saveChess.txt", ios_base::out | ios_base::binary);
		if (f.is_open()) {
			f << _strdup(s.str().c_str()) << endl;
		}
		f.close();
	}
}


		

void Board::resetSaveChess() {
	fstream f;
	f.open("../saveChess.txt", ios_base::out | ios_base::app | ios_base::binary);
	if (f.is_open()) {
		f.clear();
	}
	f.close();
}
void Board::display(sf::Color c1, sf::Color c2) {
	for (int i = 0; i < 8; i++)
	{
		bool tempCol = (i % 2 == 0);
		for (int j = 0; j < 8; j++)
		{
			square[j + i * 8].setPosition(sf::Vector2f(j * 96.f+96.f, i * 64.f+64.f));
			square[j + i * 8].setSize(sf::Vector2f(96.f, 64.f));
			square[j + i * 8].setColor(tempCol ? c1 : c2);

			tempCol = !tempCol;
		}
	}
}

void Board::massiveSetColor(vector<int> vct, sf::Color c1) {
	for (int i = 0; i < vct.size(); i++)
	{
		this->square[vct[i]].setColor(c1);
	}
}

void Board::setPiece(int x, int y, ChessPiece* cp){
	this->square[x * 8 + y].setPiece(cp);
}


string Board::log(int i) {
	string t;
	switch (i%8) {
	case 0: 
		t= "a"; break;
	case 1:
		t= "b"; break;
	case 2:
		t= "c"; break;
	case 3:
		t= "d"; break;
	case 4:
		t= "e"; break;
	case 5:
		t= "f"; break;
	case 6:
		t= "g"; break;
	case 7:
		t= "h"; break;
	default:
		break;
	}
	return  t + to_string(8 - i / 8) ;
}
int Board::unlog(string position)
{
	char temp = position[0];
	int i = 0;
	if (temp == 'a')
		i = 0;
	if (temp == 'b')
		i = 1;
	if (temp == 'c')
		i = 2;
	if (temp == 'd')
		i = 3;
	if (temp == 'e')
		i = 4;
	if (temp == 'f')
		i = 5;
	if (temp == 'g')
		i = 6;
	if (temp == 'h')
		i = 7;
	return i + (8-int(position[1]-48))*8; 
}
//-------------------------------------------------------

//------GAME HANDLING--------
void Board::moveHandle(sf::Vector2f v) {
	stringstream s;
	//include 5 cases after a move is completed: 
	//1. Normal: nothing special
	//2. Checking: a king is attacked
	//3. Hidden check: a king is covered from an attack and the covering piece can't move
	//4. Pawn's promotion: a pawn reached the other side.
	//5. Stalemate draw(rarely appear): the king is NOT in check, but no piece can be moved without putting the king in check

	//set color to original
	this->display(sf::Color{ 250,245,246 }, sf::Color{ 255,142,176 });

	//finding pressed cell:
	for (int i = 0; i < 64; i++)
	{

		if (!this->square[i].getBounds().contains(v)) continue;

		if (this->isMassiveChose()) continue;
		ChessPiece* piece = this->square[i].getPiece();

		//pressed a piece:

		if (piece && this->isBlackTurn == piece->isBlackSide())
		{
			//clear possible move & kill of the old piece
			if (this->cellPressed != -1)
			{
				this->square[this->cellPressed].getPiece()->clear();
			}

			//calculating possible move & possible kill:
			this->cellPressed = i; //save pressed piece

			this->square[i].setColor(sf::Color(96, 152, 176));

			s << "Press cell: " << this->log(i) << endl;//log check

			//move calculator for pressed piece:

			piece->possibleMove(blackCoords, whiteCoords);

			this->checkKing(piece);

			//special case - pressed King
			if (dynamic_cast<King*>(this->square[this->cellPressed].getPiece()))
			{
				//cout << "Handling King! \n";
				if (this->Checked_(dynamic_cast<King*>(this->square[this->cellPressed].getPiece())) == false) {
					this->possibleCastle(piece, blackCoords, whiteCoords);
				}
				this->possibleCheck();

			}

			if (dynamic_cast<Pawn*>(this->square[this->cellPressed].getPiece())) {
				this->crossKillSpecial(dynamic_cast<Pawn*>(this->square[this->cellPressed].getPiece()), blackCoords, whiteCoords);
			}

			//Setting color:
			this->massiveSetColor(piece->getMove(), sf::Color(189, 231, 236));
			this->massiveSetColor(piece->getKill(), sf::Color(223, 33, 90));
			break;	//skip the code below
		}

		// pressed a cell that is included in vector move or kill of cellPressed:
		if (this->cellPressed != -1 && this->moveIncluded(i))
		{
			//complete the move:
			s << "Move completed \n";
			// Save the reference to the piece at the destination cell (if any)
			ChessPiece* destinationPiece = this->square[i].getPiece();

			//complete the move
			if (dynamic_cast<King*>(this->square[this->cellPressed].getPiece()) && dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getIsMove() == false)
			{
				MoveMent temp;
				int x_ = dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getPosition_x();
				int y_ = dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getPosition_y();
				this->pieceShifting(i);

				if (i == x_ * 8 + y_ + 2) {
					ChessPiece* Rook_ = this->square[x_ * 8 + y_ + 3].getPiece();
					temp.setC1(this->square[x_ * 8 + y_ + 3]);
					temp.setC1_(x_ * 8 + y_ + 3);
					temp.setC2_(x_ * 8 + y_ + 1);
					temp.setC2(this->square[x_ * 8 + y_ + 1]);

					Rook_->setPosition(x_, y_ + 1); // set new coordinate for piece
					this->square[x_ * 8 + y_ + 1].setPiece(Rook_); // set piece for cell
					this->square[x_ * 8 + y_ + 3].setPiece(nullptr);
					this->resetMove();
					this->allMoves.push_back(temp);
					this->sizeMove++;
					this->moveAt++;
					s << "Castle: \n\tKing: " << this->log(x_ * 8 + y_) << " to " << this->log(x_ * 8 + y_ + 2) << "\n\tRook: " << this->log(x_ * 8 + y_ + 3) << " to " << this->log(x_ * 8 + y_ + 1) << endl;
				}
				else if (i == x_ * 8 + y_ - 2) {
					ChessPiece* Rook_ = this->square[x_ * 8 + y_ - 4].getPiece();
					temp.setC1(this->square[x_ * 8 + y_ - 4]);
					temp.setC1_(x_ * 8 + y_ - 4);
					temp.setC2_(x_ * 8 + y_ - 1);
					temp.setC2(this->square[x_ * 8 + y_ - 1]);
					Rook_->setPosition(x_, y_ - 1); // set new coordinate for piece
					this->square[x_ * 8 + y_ - 1].setPiece(Rook_); // set piece for cell
					this->square[x_ * 8 + y_ - 4].setPiece(nullptr);
					this->resetMove();
					this->allMoves.push_back(temp);
					this->sizeMove++;
					this->moveAt++;
					s << "Castle: \n\tKing: " << this->log(x_ * 8 + y_) << " to " << this->log(x_ * 8 + y_ - 2) << "\n\tRook: " << this->log(x_ * 8 + y_ - 4) << " to " << this->log(x_ * 8 + y_ - 1) << endl;
				}
				this->sound->playCastleSound();
			}
			else
			{
				this->pieceShifting(i);
			}
			this->numMove++;


			// check if a piece is killed

			bool pieceKilled = (destinationPiece != nullptr);			// Play corresponding sound
			if (pieceKilled) {
				this->sound->playKillSound();
				this->square[i].getPiece()->addKill(i); // Add the index of the killed piece
			}
			else {
				this->sound->playMoveSound();
			}

			this->blackCoords.clear();
			this->whiteCoords.clear();
			//re-get coordinates of black and white pieces on the board

			this->mapCalculate();
			this->checkMate();
			if (this->checkMate_ != 0)
				s << "\tCheck mate !!!" << endl;
			this->saveChess(s);
			//this->print();
			break;
		}
		//if pressed a cell that is not included
		if (this->cellPressed != -1 && !this->moveIncluded(i))
		{
			//clear all old piece's vector
			this->square[this->cellPressed].getPiece()->clear();
			break;
		}
	}


}



void Board::moveHandleForComputer(sf::Vector2f v) {
	stringstream s;
	//include 5 cases after a move is completed: 
	//1. Normal: nothing special
	//2. Checking: a king is attacked
	//3. Hidden check: a king is covered from an attack and the covering piece can't move
	//4. Pawn's promotion: a pawn reached the other side.
	//5. Stalemate draw(rarely appear): the king is NOT in check, but no piece can be moved without putting the king in check

	//set color to original
	this->display(sf::Color{ 250,245,246 }, sf::Color{ 255,142,176 });

	//finding pressed cell:
	for (int i = 0; i < 64; i++)
	{

		if (!this->square[i].getBounds().contains(v)) continue;

		if (this->isMassiveChose()) continue;
		ChessPiece* piece = this->square[i].getPiece();

		//pressed a piece:

		if (piece && this->isBlackTurn == piece->isBlackSide())
		{
			//clear possible move & kill of the old piece
			if (this->cellPressed != -1)
			{
				this->square[this->cellPressed].getPiece()->clear();
			}

			//calculating possible move & possible kill:
			this->cellPressed = i; //save pressed piece

			this->square[i].setColor(sf::Color(96, 152, 176));

			s << "Press cell: " << this->log(i) << endl;//log check

			//move calculator for pressed piece:

			piece->possibleMove(blackCoords, whiteCoords);

			this->checkKing(piece);

			//special case - pressed King
			if (dynamic_cast<King*>(this->square[this->cellPressed].getPiece()))
			{
				//cout << "Handling King! \n";
				if (this->Checked_(dynamic_cast<King*>(this->square[this->cellPressed].getPiece())) == false) {
					this->possibleCastle(piece, blackCoords, whiteCoords);
				}


			}

			if (dynamic_cast<Pawn*>(this->square[this->cellPressed].getPiece())) {
				this->crossKillSpecial(dynamic_cast<Pawn*>(this->square[this->cellPressed].getPiece()), blackCoords, whiteCoords);
			}

			//Setting color:
			this->massiveSetColor(piece->getMove(), sf::Color(189, 231, 236));
			this->massiveSetColor(piece->getKill(), sf::Color(223, 33, 90));
			break;	//skip the code below
		}
		else if (this->isBlackTurn == true) {
			string position;
			if (this->allMoves.size() != 0) {
				if ((this->allMoves[allMoves.size() - 1].getC1_() == 63 && this->allMoves[allMoves.size() - 1].getC2_() == 61) || (this->allMoves[allMoves.size() - 1].getC1_() == 56 && this->allMoves[allMoves.size() - 1].getC2_() == 59))
				{
					position = Connector::getNextMove(log(this->allMoves[allMoves.size() - 2].getC1_()) + log(this->allMoves[allMoves.size() - 2].getC2_()));
				}
				else
				{
					position = Connector::getNextMove(log(this->allMoves[allMoves.size() - 1].getC1_()) + log(this->allMoves[allMoves.size() - 1].getC2_()));
				}
			}
			else
				return;

			if (position == "error")
			{
				this->checkMate_ = 1;
				return;
			}
			int i = unlog(position.substr(0, 2));
			int y = unlog(position.substr(2, 2));
			this->cellPressed = i;
			if (dynamic_cast<King*>(this->square[this->cellPressed].getPiece()) && dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getIsMove() == false)
			{
				MoveMent temp;
				int x_ = dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getPosition_x();
				int y_ = dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getPosition_y();
				this->pieceShifting(i);

				if (y == x_ * 8 + y_ + 2) {
					ChessPiece* Rook_ = this->square[x_ * 8 + y_ + 3].getPiece();
					temp.setC1(this->square[x_ * 8 + y_ + 3]);
					temp.setC1_(x_ * 8 + y_ + 3);
					temp.setC2_(x_ * 8 + y_ + 1);
					temp.setC2(this->square[x_ * 8 + y_ + 1]);

					Rook_->setPosition(x_, y_ + 1); // set new coordinate for piece
					this->square[x_ * 8 + y_ + 1].setPiece(Rook_); // set piece for cell
					this->square[x_ * 8 + y_ + 3].setPiece(nullptr);
					this->resetMove();
					this->allMoves.push_back(temp);
					this->sizeMove++;
					this->moveAt++;
					s << "Castle: \n\tKing: " << this->log(x_ * 8 + y_) << " to " << this->log(x_ * 8 + y_ + 2) << "\n\tRook: " << this->log(x_ * 8 + y_ + 3) << " to " << this->log(x_ * 8 + y_ + 1) << endl;
				}
				else if (y == x_ * 8 + y_ - 2) {
					ChessPiece* Rook_ = this->square[x_ * 8 + y_ - 4].getPiece();
					temp.setC1(this->square[x_ * 8 + y_ - 4]);
					temp.setC1_(x_ * 8 + y_ - 4);
					temp.setC2_(x_ * 8 + y_ - 1);
					temp.setC2(this->square[x_ * 8 + y_ - 1]);
					Rook_->setPosition(x_, y_ - 1); // set new coordinate for piece
					this->square[x_ * 8 + y_ - 1].setPiece(Rook_); // set piece for cell
					this->square[x_ * 8 + y_ - 4].setPiece(nullptr);
					this->resetMove();
					this->allMoves.push_back(temp);
					this->sizeMove++;
					this->moveAt++;
					s << "Castle: \n\tKing: " << this->log(x_ * 8 + y_) << " to " << this->log(x_ * 8 + y_ - 2) << "\n\tRook: " << this->log(x_ * 8 + y_ - 4) << " to " << this->log(x_ * 8 + y_ - 1) << endl;
				}
				this->sound->playCastleSound();
			}
			else
			{
				this->pieceShifting(y);
			}
			this->blackCoords.clear();
			this->whiteCoords.clear();
			this->mapCalculate();
			continue;
		}

		// pressed a cell that is included in vector move or kill of cellPressed:
		if (this->cellPressed != -1 && this->moveIncluded(i))
		{
			//complete the move:
			s << "Move completed \n";
			// Save the reference to the piece at the destination cell (if any)
			ChessPiece* destinationPiece = this->square[i].getPiece();

			//complete the move
			if (dynamic_cast<King*>(this->square[this->cellPressed].getPiece()) && dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getIsMove() == false)
			{
				MoveMent temp;
				int x_ = dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getPosition_x();
				int y_ = dynamic_cast<King*>(this->square[this->cellPressed].getPiece())->getPosition_y();
				this->pieceShifting(i);

				if (i == x_ * 8 + y_ + 2) {
					ChessPiece* Rook_ = this->square[x_ * 8 + y_ + 3].getPiece();
					temp.setC1(this->square[x_ * 8 + y_ + 3]);
					temp.setC1_(x_ * 8 + y_ + 3);
					temp.setC2_(x_ * 8 + y_ + 1);
					temp.setC2(this->square[x_ * 8 + y_ + 1]);

					Rook_->setPosition(x_, y_ + 1); // set new coordinate for piece
					this->square[x_ * 8 + y_ + 1].setPiece(Rook_); // set piece for cell
					this->square[x_ * 8 + y_ + 3].setPiece(nullptr);
					this->resetMove();
					this->allMoves.push_back(temp);
					this->sizeMove++;
					this->moveAt++;
					s << "Castle: \n\tKing: " << this->log(x_ * 8 + y_) << " to " << this->log(x_ * 8 + y_ + 2) << "\n\tRook: " << this->log(x_ * 8 + y_ + 3) << " to " << this->log(x_ * 8 + y_ + 1) << endl;
				}
				else if (i == x_ * 8 + y_ - 2) {
					ChessPiece* Rook_ = this->square[x_ * 8 + y_ - 4].getPiece();
					temp.setC1(this->square[x_ * 8 + y_ - 4]);
					temp.setC1_(x_ * 8 + y_ - 4);
					temp.setC2_(x_ * 8 + y_ - 1);
					temp.setC2(this->square[x_ * 8 + y_ - 1]);
					Rook_->setPosition(x_, y_ - 1); // set new coordinate for piece
					this->square[x_ * 8 + y_ - 1].setPiece(Rook_); // set piece for cell
					this->square[x_ * 8 + y_ - 4].setPiece(nullptr);
					this->resetMove();
					this->allMoves.push_back(temp);
					this->sizeMove++;
					this->moveAt++;
					s << "Castle: \n\tKing: " << this->log(x_ * 8 + y_) << " to " << this->log(x_ * 8 + y_ - 2) << "\n\tRook: " << this->log(x_ * 8 + y_ - 4) << " to " << this->log(x_ * 8 + y_ - 1) << endl;
				}
				this->sound->playCastleSound();
			}
			else
			{
				this->pieceShifting(i);
			}
			this->numMove++;


			// check if a piece is killed

			bool pieceKilled = (destinationPiece != nullptr);			// Play corresponding sound
			if (pieceKilled) {
				this->sound->playKillSound();
				this->square[i].getPiece()->addKill(i); // Add the index of the killed piece
			}
			else {
				this->sound->playMoveSound();
			}

			this->blackCoords.clear();
			this->whiteCoords.clear();
			//re-get coordinates of black and white pieces on the board

			this->mapCalculate();
			this->checkMate();
			if (this->checkMate_ != 0)
				s << "\tCheck mate !!!" << endl;
			this->saveChess(s);
			//this->print();
			break;
		}
		//if pressed a cell that is not included
		if (this->cellPressed != -1 && !this->moveIncluded(i))
		{
			//clear all old piece's vector
			this->square[this->cellPressed].getPiece()->clear();
			break;
		}

	}
}

void Board::crossKillSpecial(ChessPiece* cp, vector<int> bCoords, vector<int> wCoords) {
	int x = cp->getPosition_x();
	int y = cp->getPosition_y();

	if (dynamic_cast<Pawn*>(this->square[x * 8 + y + 1].getPiece())) {
		Pawn* pawn = dynamic_cast<Pawn*>(this->square[x * 8 + y + 1].getPiece());
		if (pawn->isBlackSide()) {
			if (!cp->isBlackSide())
			{
				if (pawn->getNumMoveFirst() == 2 && this->numMove - pawn->getNumMove() == 1) {
					dynamic_cast<Pawn*>(cp)->addKill((x -1) * 8 + y + 1);
				}
			}
		}
		else if (!pawn->isBlackSide()) {
			if (cp->isBlackSide())
			{
				if (pawn->getNumMoveFirst() == 2 && this->numMove - pawn->getNumMove() == 1) {
					dynamic_cast<Pawn*>(cp)->addKill((x + 1) * 8 + y + 1);
				}
			}
		}
	}
	
	if (dynamic_cast<Pawn*>(this->square[x * 8 + y - 1].getPiece())) {
		Pawn* pawn = dynamic_cast<Pawn*>(this->square[x * 8 + y - 1].getPiece());
		if (pawn->isBlackSide()) {
			if (!cp->isBlackSide()) {
				if (pawn->getNumMoveFirst() == 2 && this->numMove - pawn->getNumMove() == 1) {
					dynamic_cast<Pawn*>(cp)->addKill((x - 1) * 8 + y - 1);
				}
			}
		}
		else if (!pawn->isBlackSide()) {
			if (cp->isBlackSide()) {
				if (pawn->getNumMoveFirst() == 2 && this->numMove - pawn->getNumMove() == 1) {
					dynamic_cast<Pawn*>(cp)->addKill((x + 1) * 8 + y - 1);
				}
			}
		}
	}
}

void Board::possibleCastle(ChessPiece* cp,vector<int> bCoords, vector<int> wCoords) {
	//dynamic_cast<King*>
	int x = cp->getPosition_x();
	int y = cp->getPosition_y();

	if (cp->getIsMove() == false) {
		pair<int, int>* casesCastle = new pair<int, int>[5];
		casesCastle[0] = make_pair(0, 1);
		casesCastle[1] = make_pair(0, -1);

		bool flag = false;
		for (int k = 1; k < 3; k++)
		{
			if (this->square[x * 8 + y + 3].getPiece()->getIsMove() == false) {
				int x2 = x + casesCastle[0].first * k;
				int y2 = y + casesCastle[0].second * k;
				cp->moveCal(bCoords, wCoords, x2, y2, flag);
			}
		}

		bool flag1 = false;
		for (int k = 1; k < 3; k++)
		{
			if (this->square[x * 8 + y - 4].getPiece()->getIsMove() == false)
			{
				int x2 = x + casesCastle[1].first * k;
				int y2 = y + casesCastle[1].second * k;
				if (k == 1) {
					cp->moveCal(bCoords, wCoords, x2, y2, flag1);
				}
				else
				{
					if (this->square[x * 8 + y - 3].getPiece() == nullptr) {
						cp->moveCal(bCoords, wCoords, x2, y2, flag1);
					}
				}
			}
		}
	}

}

void Board::checkKing(ChessPiece* cp) {
	vector<int> temp;
	bool flag = false;
	
	vector<int> tempCp;
	tempCp = cp->getMove();
	tempCp = intersect(tempCp, cp->getKill());
	if (tempCp.size() == 0) {
		tempCp.push_back(-1);
	}
	vector<int> tempCp1;
	
	
	if (cp->isBlackSide())
	{
		int posCP = -1;
		int posI = -1;
		// search position chesspiece (this)
		
		for (int i=0;i<this->blackCoords.size();i++)
		{
			if (this->square[this->blackCoords[i]].getPiece() == cp) {
				posCP = this->blackCoords[i];
				posI = i;
				break;
			}
		}

		//calculate all possible move of white side
		ChessPiece* piece = this->square[this->whiteCoords[0]].getPiece();
		piece->possibleMove(this->blackCoords, this->whiteCoords);

		if (!dynamic_cast<Pawn*>(piece))
		{
			temp = piece->getMove();
		}
		else temp = piece->getGuard();

		temp = intersect(temp, piece->getGuard());

		piece->clear(); //remember to clear the created data

		for (int i = 1; i < this->whiteCoords.size(); i++)
		{
			ChessPiece* piece = this->square[this->whiteCoords[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			if (!dynamic_cast<Pawn*>(piece))
			{
				temp = intersect(temp, piece->getMove());
			}
			temp = intersect(temp, piece->getGuard());

			piece->clear();//remember to clear the created data
		}

		this->blackCoords.erase(this->blackCoords.begin() + posI);
		tempCp1 = tempCp;
		for (auto pos : tempCp) {
			auto temp1 = pos;
			bool checkKill = false;
			bool check = false;
			int posK = -1;


			this->blackCoords.push_back(pos);
			for (int i = 0; i < this->whiteCoords.size(); i++) {
				if (this->whiteCoords[i] == this->blackCoords[this->blackCoords.size() - 1]) {
					checkKill = true;
					for (int i = 0; i < this->whiteCoords.size(); i++) {
						if (this->whiteCoords[i] == pos) {
							this->whiteCoords.erase(this->whiteCoords.begin() + i);
						}
					}
				}
			}
			if (dynamic_cast<King*>(cp)) {
				posK = pos;
			}
			for (int i = 0; i < this->whiteCoords.size(); i++) {
				this->square[this->whiteCoords[i]].getPiece()->possibleMove(this->blackCoords,this->whiteCoords);
				if (this->isChecked_(this->square[this->whiteCoords[i]].getPiece(),posK))
				{
					check = true;
				}
			
				this->square[this->whiteCoords[i]].getPiece()->clear();
			}

			this->blackCoords.pop_back();

			
			if (check == false) {
				for (int i = 0; i < tempCp1.size(); i++) {
					if (tempCp1[i] == pos) {
						tempCp1.erase(tempCp1.begin() + i);
					}
				}
			}

			if (checkKill == true) {
				this->whiteCoords.push_back(pos);
			}
			
		}
		this->blackCoords.push_back(posCP);
	}
	else {
		int posCP = -1;
		int posI = -1;
		// search position chesspiece (this)

		for (int i = 0; i < this->whiteCoords.size(); i++)
		{
			if (this->square[this->whiteCoords[i]].getPiece() == cp) {
				posCP = this->whiteCoords[i];
				posI = i;
				break;
			}
		}

		//calculate all possible move of black side
		ChessPiece* piece = this->square[this->blackCoords[0]].getPiece();
		piece->possibleMove(this->blackCoords, this->whiteCoords);
		if (!dynamic_cast<Pawn*>(piece))
		{
			temp = piece->getMove();
		}
		else temp = piece->getGuard();
		temp = intersect(temp, piece->getGuard());
		piece->clear(); //remember to clear the created data

		for (int i = 1; i < this->blackCoords.size(); i++)
		{
			ChessPiece* piece = this->square[this->blackCoords[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			if (!dynamic_cast<Pawn*>(piece))
			{
				temp = intersect(temp, piece->getMove());
			}
			temp = intersect(temp, piece->getGuard());
			piece->clear();//remember to clear the created data
		}
		this->whiteCoords.erase(this->whiteCoords.begin() + posI);

		tempCp1 = tempCp;
		for (auto pos : tempCp) {
			bool check = false;
			auto temp1 = pos;
			bool checkKill = false;
			this->whiteCoords.push_back(pos);
			int posK = -1;
			
			
			
			for (int i = 0; i < this->blackCoords.size(); i++) {
				if (this->blackCoords[i] == this->whiteCoords[this->whiteCoords.size() - 1]) {
					checkKill = true;
					for (int i = 0; i < this->blackCoords.size(); i++) {
						if (this->blackCoords[i] == pos) {
							this->blackCoords.erase(this->blackCoords.begin() + i);
						}
					}
				}
			}

			if (dynamic_cast<King*>(cp)) {
				posK = pos;
			}
			for (int i = 0; i < this->blackCoords.size(); i++) {
				
				this->square[this->blackCoords[i]].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
				if (this->isChecked_(this->square[this->blackCoords[i]].getPiece(),posK))
				{
					check = true;
				}
				
				this->square[this->blackCoords[i]].getPiece()->clear();
			}
			
			this->whiteCoords.pop_back();

			if (check == false) {
				for (int i = 0; i < tempCp1.size(); i++) {
					if (tempCp1[i] == pos) {
						tempCp1.erase(tempCp1.begin() + i);
					}
				}
			}

			if (checkKill == true) {
				this->blackCoords.push_back(pos);
			}

			
		}
		this->whiteCoords.push_back(posCP);
	}
	//so temp is the final vector, with i is the move to check
	cp->clearUnableMove(tempCp1);
	temp.clear();
}

void Board::checkMate() {
	vector<int> temp;
	if (this->isBlackTurn) {
		vector<int> blackCoords_ = this->blackCoords;
		for (int i = 0; i < blackCoords_.size(); i++)
		{
			ChessPiece* piece = this->square[blackCoords_[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			this->checkKing(piece);
			temp = intersect(temp, piece->getMove());
			temp = intersect(temp, piece->getKill());
			piece->clear();//remember to clear the created data
		}
		if (temp.size() == 0) {
			this->checkMate_ = 1;
		}
	}
	else
	{
		vector<int> whiteCoords_ = this->whiteCoords;
		for (int i = 0; i < whiteCoords_.size(); i++)
		{
			ChessPiece* piece = this->square[whiteCoords_[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			this->checkKing(piece);
			temp = intersect(temp, piece->getMove());
			temp = intersect(temp, piece->getKill());
			piece->clear();//remember to clear the created data
		}
		if (temp.size() == 0) {
			this->checkMate_ = -1;
		}
	}
}
bool Board::Checked_(ChessPiece* king) {
	vector<int> temp;
	bool flag = false;

	if (king->isBlackSide())
	{
		//calculate all possible move of white side
		ChessPiece* piece = this->square[this->whiteCoords[0]].getPiece();
		piece->possibleMove(this->blackCoords, this->whiteCoords);

	

		for (int i = 0; i < this->whiteCoords.size(); i++)
		{
			ChessPiece* piece = this->square[this->whiteCoords[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			
			temp = intersect(temp, piece->getKill());

			piece->clear();//remember to clear the created data
		}
	}
	else {
		//calculate all possible move of black side
		ChessPiece* piece = this->square[this->blackCoords[0]].getPiece();
		piece->possibleMove(this->blackCoords, this->whiteCoords);
		
		for (int i = 0; i < this->blackCoords.size(); i++)
		{
			ChessPiece* piece = this->square[this->blackCoords[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			
			temp = intersect(temp, piece->getKill());
			piece->clear();//remember to clear the created data
		}
		/*for (int i = 0; i < temp.size(); i++) {

		}*/
	}

	for (int i = 0; i < temp.size(); i++) {
		if (king->getPosition_x() * 8 + king->getPosition_y() == temp[i]) {
			return true;
		}
	}

	temp.clear();
	return false;
}
void Board::Promote_(ChessPiece* cp, MoveMent& move) {
	int i = (cp->getPosition_x() * 8 + cp->getPosition_y());
	int x = i / 8;
	int y = i % 8;
	int press=-1;
	bool flag = false;
	if (x == 0)
	{
		Promote promote("../Img/Menu/promoteWhite.png");

		sf::RenderWindow window(sf::VideoMode(400, 100), "Promote", sf::Style::Titlebar|sf::Style::None);
		window.setVerticalSyncEnabled(true);
		window.draw(promote);
		window.display();
		
		sf::Vector2i pos = window.getPosition();
		pos.x = 875;
		pos.y = 460;

		window.setPosition(pos);
		
		while (window.isOpen()) {
			sf::Event event;

			while (window.pollEvent(event))
			{
				//Event handler
				if (event.type == sf::Event::MouseButtonPressed)
				{
					//Press in "Start" button	
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

						if (promote.queenBounds().contains(mouse) && !promote.getState())
						{
							cp = new Queen();
							cp->setPiece("../Img/Texture/whiteQueenn.png", x, y, false, "Queen");
							this->square[i].setPiece(cp);
							//this->setPiece(x, y, cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}

						//sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
						//press in PvP button
						else if (promote.rookBounds().contains(mouse) && !promote.getState())
						{
							cp = new Rook();
							cp->setPiece("../Img/Texture/whiteRookk.png", x, y, false, "Rook");
							//cp->setIsMove();
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}

						//sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

						else if (promote.knightBounds().contains(mouse) && !promote.getState())
						{
							cp = new Knight();
							cp->setPiece("../Img/Texture/whiteKnightt.png", x, y, false, "Knight");
							//cp->setIsMove();
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}

						//sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
						//press in PvP button
						else if (promote.bishopBounds().contains(mouse) && !promote.getState())
						{
							cp = new Bishop();
							cp->setPiece("../Img/Texture/whiteBishopp.png", x, y, false, "Bishop");
							//cp->setIsMove();
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}

						else
						{
							continue;
						}
					}

				}
			}
		}
	}
	else if (x == 7)
	{
		Promote promote("../Img/Menu/promoteBlack.png");

		sf::RenderWindow window(sf::VideoMode(400, 100), "Promote",sf::Style::Titlebar| sf::Style::None);
		window.setVerticalSyncEnabled(true);
		window.draw(promote);
		window.display();
		sf::Vector2i pos = window.getPosition();
		pos.x = 875;
		pos.y = 460;

		window.setPosition(pos);

		while (window.isOpen()) {
			sf::Event event;

			while (window.pollEvent(event))
			{
				//Event handler
				if (event.type == sf::Event::MouseButtonPressed)
				{
					//Press in "Start" button	
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

						if (promote.queenBounds().contains(mouse) && !promote.getState())
						{
							cp = new Queen();
							cp->setPiece("../Img/Texture/blackQueenn.png", x, y, true, "Queen");
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}


						//sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
						//press in PvP button
						else if (promote.rookBounds().contains(mouse) && !promote.getState())
						{
							cp = new Rook();
							cp->setPiece("../Img/Texture/blackRookk.png", x, y, true, "Rook");
							//cp->setIsMove();
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}

						//sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

						else if (promote.knightBounds().contains(mouse) && !promote.getState())
						{
							cp = new Knight();
							cp->setPiece("../Img/Texture/blackKnightt.png", x, y, true, "Knight");
							//cp->setIsMove();
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}


						//sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
						//press in PvP button
						else if (promote.bishopBounds().contains(mouse) && !promote.getState())
						{
							cp = new Bishop();
							cp->setPiece("../Img/Texture/blackBishopp.png", x, y, true, "Bishop");
							//cp->setIsMove();
							this->square[i].setPiece(cp);
							move.setTempMove(this->square[i]);
							move.setTempMove_(i);
							window.close();
						}
						else
						{
							continue;
						}
					}
				}

			}
		}
	}
}
//------SUPPORT MOVE HANDLING-------

bool Board::isMassiveChose() {
	int count = 0;
	for (int i = 0; i < 64; i++)
	{
		if (this->square[i].getColor() == sf::Color::Green)
		{
			count++;
		}
		if (count ==1)
		{
			return true;
		}
	}
	return false;
}

bool Board::moveIncluded(int i) {
	ChessPiece* cp = this->square[this->cellPressed].getPiece();

	for (int k = 0; k < cp->getMove().size(); k++)
	{
		if (i == cp->getMove()[k])
		{
			return true;
		}
	}
	
	for (int k = 0; k < cp->getKill().size(); k++)
	{
		if (i == cp->getKill()[k])
		{
			return true;
		}
	}
	return false;
}


void Board::pieceShifting(int i) {
	stringstream s;
	MoveMent move;
	s << "Try shifting, cell: "<< this->log(this->cellPressed) << " to "<< this->log(i) << " \n";
	ChessPiece* cp = this->square[this->cellPressed].getPiece();
	ChessCell* square_temp = new ChessCell(this->square[this->cellPressed]);
	
	//int v = this->cellPressed;
	move.setC1(this->square[this->cellPressed]);
	move.setC1_(this->cellPressed);
	move.setC2(this->square[i]);
	move.setC2_(i);


	if (this->square[i].getPiece() == nullptr && dynamic_cast<Pawn*>(cp)) {
		if (cp->isBlackSide()) {
			if (this->square[i - 8].getPiece() != nullptr && this->square[i - 8].getPiece() != cp)
			{
				move.setTempKill(this->square[i - 8]);
				move.setTempKill_(i - 8);
				this->square[i - 8].setPiece(nullptr);
			}
		}
		else 
		{
			if (this->square[i + 8].getPiece() != nullptr && this->square[i + 8].getPiece() != cp)
			{
				move.setTempKill(this->square[i + 8]);
				move.setTempKill_(i + 8);
				this->square[i + 8].setPiece(nullptr);
			}

		}

		cp->setPosition(i / 8, i % 8); // set new coordinate for piece
		this->square[i].setPiece(cp); // set piece for cell
		this->square[this->cellPressed].setPiece(nullptr);

	}
	else {
		if (move.getC2().getPiece() != nullptr)
		{
			move.setTempKill(this->square[i]);
			move.setTempKill_(i);
		}
		//switch position:
		cp->setPosition(i / 8, i % 8); // set new coordinate for piece
		this->square[i].setPiece(cp); // set piece for cell
		this->square[this->cellPressed].setPiece(nullptr); //set old position's piece to nullptr
	}
	if (dynamic_cast<King*>(cp)) {
		dynamic_cast<King*>(cp)->setIsMove(true);

	}
	if (dynamic_cast<Rook*>(cp)) {
		dynamic_cast<Rook*>(cp)->setIsMove(true);
	}

	if (dynamic_cast<Pawn*>(cp))
	{
		if (abs(i - this->cellPressed) == 16)
		{
			dynamic_cast<Pawn*>(cp)->setNumMoveFirst(2);
		}
		else dynamic_cast<Pawn*>(cp)->setNumMoveFirst(1);
		dynamic_cast<Pawn*>(cp)->setNumMove(this->numMove);
		Promote_(cp,move);
	}

	this->cellPressed = i; //cellPressed coordinate is now i
	this->square[this->cellPressed].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
	if (this->isChecked(this->square[this->cellPressed].getPiece()))
	{
		s << "King is check! \n";
		//this->state = 1;
	}
	this->resetMove();
	this->allMoves.push_back(move);
	this->sizeMove++;
	this->moveAt++;
	this->square[this->cellPressed].getPiece()->clear(); //clear move, kill & guard vector of that piece
	//----switch position finished-----

	//change turn:
	this->isBlackTurn = !this->isBlackTurn;
	//reset
	if (this->cellPressed == -1)
	{
		return;
	}
	this->cellPressed = -1;

	this->saveChess(s);
}

//-----Relevant to King's moves: ----- 

void Board::possibleCheck() {
	vector<int> temp;
	bool flag = false;
	ChessPiece* king = this->square[this->cellPressed].getPiece();

	if (king->isBlackSide())
	{
		//calculate all possible move of white side
		ChessPiece* piece = this->square[this->whiteCoords[0]].getPiece();
		piece->possibleMove(this->blackCoords, this->whiteCoords);

		if (!dynamic_cast<Pawn*>(piece))
		{
			temp = piece->getMove();
		}
		else temp = piece->getGuard();

		temp = intersect(temp, piece->getGuard());

		piece->clear(); //remember to clear the created data

		for (int i = 1; i < this->whiteCoords.size(); i++)
		{
			ChessPiece* piece = this->square[this->whiteCoords[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			if (!dynamic_cast<Pawn*>(piece))
			{
				temp = intersect(temp, piece->getMove());
			}
			temp = intersect(temp, piece->getGuard());

			piece->clear();//remember to clear the created data
		}

		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == 3) {
				temp.push_back(2);
			
				break;
			}
			else if (temp[i] == 5) {
				temp.push_back(6);
				break;
			}
		}

		
	}
	else {
		//calculate all possible move of black side
		ChessPiece* piece = this->square[this->blackCoords[0]].getPiece();
		piece->possibleMove(this->blackCoords, this->whiteCoords);
		if (!dynamic_cast<Pawn*>(piece))
		{
			temp = piece->getMove();
		}
		else temp = piece->getGuard();		
		temp = intersect(temp, piece->getGuard());
		piece->clear(); //remember to clear the created data

		for (int i = 1; i < this->blackCoords.size(); i++)
		{
			ChessPiece* piece = this->square[this->blackCoords[i]].getPiece();
			piece->possibleMove(this->blackCoords, this->whiteCoords);
			if (!dynamic_cast<Pawn*>(piece))
			{
				temp = intersect(temp, piece->getMove());
			}
			temp = intersect(temp, piece->getGuard());
			piece->clear();//remember to clear the created data
		}

		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == 59) {
				temp.push_back(58);
				break;
			}
			else if (temp[i] == 61) {
				temp.push_back(62);
				break;
			}
		}
	}
	//so temp is the final vector, with i is the move to check
	dynamic_cast<King*>(king)->clearUnableMove(temp);
	temp.clear();
}

//--------KING'S POSSIBLE MOVE----------

//intersect of 2 vectors move
vector<int> Board::intersect(vector<int>v1, vector<int>v2) {
	vector<int> temp;
	if (v2.empty())
	{
		return v1;
	}
	if (v1.empty())
	{
		return v2;
	}
	temp = v1;
	for (int i = 0; i < v2.size(); i++)
	{
		vector<int>::iterator it = std::find(v1.begin(), v1.end(), v2[i]);
		if (it != v1.end()) continue;
		temp.push_back(v2[i]);
	}
	return temp;
}


void Board::mapCalculate() {
	//get coords of pieces:
	for (int i = 0; i < 64; i++)
	{
		ChessPiece* cp = this->square[i].getPiece();
		if (!cp) continue;
		if (cp->isBlackSide())
		{
			this->blackCoords.push_back(i);
		}
		else this->whiteCoords.push_back(i);
	}
}

//----INHERIT METHOD-------
void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (int i = 0; i < 64; i++) {
		target.draw(square[i]);
	}
}

bool Board:: isChecked(ChessPiece* cp) {

	// sound buffer setting
	
	vector<int> kill = cp->getKill();
	for (int i = 0; i < kill.size(); i++)
	{
		ChessPiece* temp = this->square[kill[i]].getPiece();
		if (dynamic_cast<King*>(temp) && cp->isBlackSide() != temp->isBlackSide())
		{
			
			this->square[kill[i]].setColor(sf::Color::Magenta);
			this->sound->playCheckedSound();
			//this->state = 1; // as checked
			return true;
		}
	}
	kill.clear();
	return false;	
}
bool Board::isChecked_(ChessPiece* cp,int pos) {

	vector<int> kill = cp->getKill();

	if (pos!=-1) {
		for (int i = 0; i < kill.size(); i++)
		{
			if (pos==kill[i])
			{
				//this->square[kill[i]].setColor(sf::Color::Magenta);
				//this->state = 1; // as checked
				return true;
			}
		}
	}
	else {
		for (int i = 0; i < kill.size(); i++)
		{
			ChessPiece* temp = this->square[kill[i]].getPiece();
			if (dynamic_cast<King*>(temp) && cp->isBlackSide() != temp->isBlackSide())
			{
				//this->square[kill[i]].setColor(sf::Color::Magenta);
				//this->state = 1; // as checked
				return true;
			}
		}
	}
	kill.clear();
	return false;
}

//int Board::stateCal(sf::Vector2f v) {
//	//state = 0: Normal state
//	//state = 1: Check
//	//state = 2: Pawn's promoting
//	//state = 3: Stalemate draw
//	switch (this->state)
//	{
//	case 0: {
//		this->moveHandle(v);
//		break;
//	}
//	case 1: {
//		//handle possible check
//
//	}
//	default:
//		break;
//	}
//	return 0;
//}



void Board::undoMove() {
	//cout << this->allMoves.size();
	//i = this->allMoves.size() - 1;

	//int i = this->allMoves.size() - 1;

	int i = this->moveAt-1;

	if (i < 0) {
		return;
	}
	
	if (dynamic_cast<Pawn*>(this->allMoves[i].getC1().getPiece()))
	{
		int c1 = this->allMoves[i].getC1_();
		int c2 = this->allMoves[i].getC2_();
		if (c2 <= 7 && c2 >= 0)
		{
			ChessPiece* pawn = new Pawn();
			pawn->setPiece("../Img/Texture/whitePawnn.png", c1/8, c1%8, false, "Pawn");
			this->square[c1].setPiece(nullptr);
			delete this->square[c1].getPiece();
			this->square[c1].setPiece(pawn); // set piece for cell
			this->square[c1].getPiece()->setPosition(c1 / 8, c1 % 8);
			
			this->square[c2].setPiece(this->allMoves[i].getC2().getPiece());

			this->square[c1].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			

			//this->square[c1].setPiece(this->allMoves[this->allMoves.size() - 1].getTempKill().getPiece());
			dynamic_cast<Pawn*>(this->square[c1].getPiece())->setNumMove(this->numMove - 1);

			if (this->sizeMove == this->allMoves.size()) {
				this->reMoves.push_back(this->allMoves[this->moveAt-1]);
				this->moveAt--;
			}
			else
				this->allMoves.pop_back();
			this->isBlackTurn = !this->isBlackTurn;



			this->blackCoords.clear();
			this->whiteCoords.clear();
			this->mapCalculate();
			return;
			
		}
		else if (c2 <= 63 && c2 >= 56) {
			ChessPiece* pawn = new Pawn();
			pawn->setPiece("../Img/Texture/blackPawnn.png", c1 / 8, c1 % 8, true, "Pawn");
			this->square[c1].setPiece(nullptr);
			delete this->square[c1].getPiece();
			this->square[c1].setPiece(pawn); // set piece for cell
			this->square[c1].getPiece()->setPosition(c1 / 8, c1 % 8);

			this->square[c2].setPiece(this->allMoves[i].getC2().getPiece());

			this->square[c1].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);


			//this->square[c1].setPiece(this->allMoves[this->allMoves.size() - 1].getTempKill().getPiece());
			dynamic_cast<Pawn*>(this->square[c1].getPiece())->setNumMove(this->numMove - 1);

		
			if (this->sizeMove == this->allMoves.size())
			{
				this->reMoves.push_back(this->allMoves[this->moveAt-1]);
				this->moveAt--;
			}
			else
				this->allMoves.pop_back();
		
			//i--;
			this->isBlackTurn = !this->isBlackTurn;

			this->blackCoords.clear();
			this->whiteCoords.clear();
			this->mapCalculate();
			return;
		}
		
		
	}

	pieceShifting_un(this->allMoves[i].getC1_(), this->allMoves[i].getC2_());
	if (i >= 1) {
		if (this->allMoves[i - 1].getC1().getPiece()->isBlackSide() == this->allMoves[i].getC1().getPiece()->isBlackSide()) {
			pieceShifting_un(this->allMoves[i - 1].getC1_(), this->allMoves[i - 1].getC2_());
			dynamic_cast<King*>(this->square[this->allMoves[i -1].getC1_()].getPiece())->setIsMove(false);
			dynamic_cast<Rook*>(this->square[this->allMoves[i].getC1_()].getPiece())->setIsMove(false);

			if (this->sizeMove == this->allMoves.size())
			{
				this->reMoves.push_back(this->allMoves[this->moveAt-1]);
				this->moveAt--;
			}
			else
				this->allMoves.pop_back();
			
			this->isBlackTurn = !this->isBlackTurn;
		}
	}
	if (this->sizeMove == this->allMoves.size())
	{
		this->reMoves.push_back(this->allMoves[this->moveAt-1]);
		this->moveAt--;
	}
	else
		this->allMoves.pop_back();
}

void Board::redoMove() {
	int i = this->reMoves.size()-1;
	if (i < 0) {
		return;
	}
	if (dynamic_cast<Pawn*>(this->reMoves[i].getC1().getPiece()))
	{
		int c1 = this->reMoves[i].getC1_();
		int c2 = this->reMoves[i].getC2_();
		if (c2 <= 7 && c2 >= 0)
		{
			/*ChessPiece* pawn = new Pawn();
			pawn->setPiece("../Img/Texture/whitePawnn.png", c1 / 8, c1 % 8, false, "Pawn");*/

			this->square[c2].setPiece(this->reMoves[i].getTempMove().getPiece()); // set piece for cell
			this->square[c2].getPiece()->setPosition(c2 / 8, c2 % 8);
			this->square[c2].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);


			this->square[c1].setPiece(nullptr);
			delete this->square[c1].getPiece();
		
			//this->square[c2].setPiece(this->allMoves[i].getC2().getPiece());

			//this->square[c1].setPiece(this->allMoves[this->allMoves.size() - 1].getTempKill().getPiece());
			//dynamic_cast<Pawn*>(this->square[c1].getPiece())->setNumMove(this->numMove - 1)
			
			this->reMoves.pop_back();
			this->moveAt++;
			this->isBlackTurn = !this->isBlackTurn;
			


			this->blackCoords.clear();
			this->whiteCoords.clear();
			this->mapCalculate();
			return;

		}
		else if (c2 <= 63 && c2 >= 56) {
			/*ChessPiece* pawn = new Pawn();
			pawn->setPiece("../Img/Texture/blackPawnn.png", c1 / 8, c1 % 8, true, "Pawn");*/
			this->square[c1].setPiece(nullptr);
			delete this->square[c1].getPiece();

			this->square[c2].setPiece(this->reMoves[i].getTempMove().getPiece()); // set piece for cell
			this->square[c2].getPiece()->setPosition(c2 / 8, c2 % 8);

			this->square[c2].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			this->reMoves.pop_back();
			this->moveAt++;
			
			this->isBlackTurn = !this->isBlackTurn;

			this->blackCoords.clear();
			this->whiteCoords.clear();
			this->mapCalculate();
			return;
		}

	}

	pieceShifting_re(this->reMoves[i].getC1_(), this->reMoves[i].getC2_());
	if (i >= 1) {
		if (this->reMoves[i - 1].getC1().getPiece()->isBlackSide() == this->reMoves[i].getC1().getPiece()->isBlackSide()) {
			pieceShifting_re(this->reMoves[i - 1].getC1_(), this->reMoves[i - 1].getC2_());
			dynamic_cast<King*>(this->square[this->reMoves[i].getC2_()].getPiece())->setIsMove(true);
			dynamic_cast<Rook*>(this->square[this->reMoves[i-1].getC2_()].getPiece())->setIsMove(true);

			this->reMoves.pop_back();
			this->moveAt++;
			this->isBlackTurn = !this->isBlackTurn;
		}
	}

	this->reMoves.pop_back();
	this->moveAt++;
}
void Board::pieceShifting_re(int i, int j) {

	ChessPiece* cp = this->square[i].getPiece();
	this->mapCalculate();

	if (dynamic_cast<Pawn*>(cp)) {
		if (this->reMoves[this->reMoves.size() - 1].getTempKill_() == j - 8 && dynamic_cast<Pawn*>(this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece()))
		{
			this->square[j].setPiece(cp); // set piece for cell
			this->square[j].getPiece()->setPosition(j / 8, j % 8);

			this->square[j].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			this->square[i].setPiece(nullptr);
			delete this->square[i].getPiece();

			this->square[j - 8].setPiece(nullptr);

		}
		else if (this->reMoves[this->reMoves.size() - 1].getTempKill_() == j + 8 && dynamic_cast<Pawn*>(this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece()))
		{

			this->square[j].setPiece(cp); // set piece for cell
			this->square[j].getPiece()->setPosition(j / 8, j % 8);

			this->square[j].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			this->square[i].setPiece(nullptr);
			delete this->square[i].getPiece();

			this->square[j + 8].setPiece(nullptr);
			//this->square[j + 8].setPiece(this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece());
			//dynamic_cast<Pawn*>(this->square[j - 8].getPiece())->setNumMoveFirst(2);
			//dynamic_cast<Pawn*>(this->square[j - 8].getPiece())->setNumMove(this->numMove - 2);
			//dynamic_cast<Pawn*>(this->square[j].getPiece())->setNumMove(this->numMove - 1);
			/*if (this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece() != nullptr) {
				this->square[j+8].setPiece(this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece());
				this->square[j+8].getPiece()->setPosition((j+8) / 8, (j+8) % 8);
			}*/
		}
		else
		{
			this->square[j].setPiece(cp); // set piece for cell
			this->square[j].getPiece()->setPosition(j / 8, j % 8);

			this->square[j].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			this->square[i].setPiece(nullptr);
			delete this->square[i].getPiece();

			//this->square[i].setPiece(this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece());
			/*if (this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece() != nullptr) {
				this->square[i].setPiece(this->reMoves[this->reMoves.size() - 1].getTempKill().getPiece());
				this->square[i].getPiece()->setPosition(i / 8, i % 8);
			}
			dynamic_cast<Pawn*>(this->square[j].getPiece())->setNumMove(this->numMove - 1);*/
		}
	}
	else {
		//switch position:
		cp->setPosition(j / 8, j % 8); // set new coordinate for piece
		this->square[j].setPiece(cp); // set piece for cell
		this->square[i].setPiece(nullptr); //set old position's piece to nullptr
		delete this->square[i].getPiece();

		//this->square[j].setPiece(this->allMoves[this->allMoves.size() - 1].getTempKill().getPiece());
	}


	this->cellPressed = i;


	this->numMove++;
	this->square[j].getPiece()->clear(); //clear move, kill & guard vector of that piece
	//----switch position finished-----

	//change turn:
	this->isBlackTurn = !this->isBlackTurn;
	this->blackCoords.clear();
	this->whiteCoords.clear();
	this->mapCalculate();
	if (this->cellPressed == -1)
	{
		return;
	}
	this->cellPressed = -1;
	//delete cp;
}

void Board::pieceShifting_un(int i,int j) {
	
	ChessPiece* cp = this->square[j].getPiece();
	this->mapCalculate();
	
	if ( dynamic_cast<Pawn*>(cp)) {
		if (this->allMoves[this->moveAt-1].getTempKill_() == j - 8 && dynamic_cast<Pawn*>(this->allMoves[this->moveAt-1].getTempKill().getPiece()))
		{
			this->square[i].setPiece(cp); // set piece for cell
			this->square[i].getPiece()->setPosition(i / 8, i % 8);

			this->square[i].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			this->square[j].setPiece(nullptr);
			delete this->square[j].getPiece();

			this->square[j - 8].setPiece(this->allMoves[this->moveAt - 1].getTempKill().getPiece());
			dynamic_cast<Pawn*>(this->square[j - 8].getPiece())->setNumMoveFirst(2);
			dynamic_cast<Pawn*>(this->square[j - 8].getPiece())->setNumMove(this->numMove - 2);
			dynamic_cast<Pawn*>(this->square[i].getPiece())->setNumMove(this->numMove - 1);
		}
		else if (this->allMoves[this->moveAt-1].getTempKill_() == j + 8 && dynamic_cast<Pawn*>(this->allMoves[this->moveAt-1].getTempKill().getPiece()))
		{

			this->square[i].setPiece(cp); // set piece for cell
			this->square[i].getPiece()->setPosition(i / 8, i % 8);

			this->square[i].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			this->square[j].setPiece(nullptr);
			delete this->square[j].getPiece();

			this->square[j + 8].setPiece(this->allMoves[this->moveAt - 1].getTempKill().getPiece());
			dynamic_cast<Pawn*>(this->square[j + 8].getPiece())->setNumMoveFirst(2);
			dynamic_cast<Pawn*>(this->square[j + 8].getPiece())->setNumMove(this->numMove - 2);
			dynamic_cast<Pawn*>(this->square[i].getPiece())->setNumMove(this->numMove - 1);
		}
		else
		{
			this->square[i].setPiece(cp); // set piece for cell
			this->square[i].getPiece()->setPosition(i / 8, i % 8);
			this->square[i].getPiece()->possibleMove(this->blackCoords, this->whiteCoords);
			dynamic_cast<Pawn*>(this->square[i].getPiece())->setNumMove(this->numMove - 1);


			this->square[j].setPiece(nullptr);
			//delete this->square[j].getPiece();
			if (this->allMoves[this->moveAt - 1].getTempKill().getPiece() != nullptr) {
				this->square[j].setPiece(this->allMoves[this->moveAt-1].getTempKill().getPiece());
				this->square[j].getPiece()->setPosition(j / 8, j % 8);
			}
			
		}


	}
	else {
		//switch position:
		cp->setPosition(i / 8, i % 8); // set new coordinate for piece
		this->square[i].setPiece(cp); // set piece for cell
		this->square[j].setPiece(nullptr); //set old position's piece to nullptr
		delete this->square[j].getPiece();

		this->square[j].setPiece(this->allMoves[this->allMoves.size() - 1].getTempKill().getPiece());
	}
	if (dynamic_cast<King*>(cp)) {
		cp->setIsMove(false);
	}
	if (dynamic_cast<Rook*>(cp)) {
		cp->setIsMove(false);
	}
	
	this->cellPressed = j;

	
	this->numMove--;
	this->square[i].getPiece()->clear(); //clear move, kill & guard vector of that piece
	//----switch position finished-----

	//change turn:
	this->isBlackTurn = !this->isBlackTurn;
	this->blackCoords.clear();
	this->whiteCoords.clear();
	this->mapCalculate();
	if (this->cellPressed == -1)
	{
		return;
	}
	this->cellPressed = -1;
}


void Board::print() {
	system("cls");
	for (auto i : this->allMoves) {
		cout << "\n\nMove: \n";
		ChessPiece* x = i.getC1().getPiece();
		ChessPiece* y = i.getC2().getPiece();
		ChessPiece* z = i.getTempKill().getPiece();
		ChessPiece* t = i.getTempMove().getPiece();
		if (x != nullptr)
			if (x->isBlackSide())
				cout << x->getName() << " black " <<i.getC1_()<<endl;
			else 
				cout << x->getName() << " white " << i.getC1_() << endl;
		else
			cout << "NULL" << " " << i.getC1_() << endl;
		if (y != nullptr)
			if (y->isBlackSide())
				cout << y->getName() << " black " << i.getC2_() << endl;
			else
				cout << y->getName() << " white " << i.getC2_() << endl;
		else
			cout << "NULL" << " " << i.getC2_() << endl;
		if (z != nullptr)
			cout << z->getName() << " " << i.getTempKill_() << endl;
		else
			cout << "NULL" << " " << i.getTempKill_() << endl;
		if (t != nullptr)
			cout << t->getName() << " " << i.getTempMove_() << endl;
		else
			cout << "NULL" << " " << i.getTempMove_() << endl;
	}
}

void Board::resetMove()
{
	int j = this->allMoves.size() - this->moveAt;
	for (int i = 0; i < j; i++) {
		this->reMoves.pop_back();
		this->allMoves.pop_back();
		this->sizeMove--;
	}
}

void Board::resetPlay() {
	for (int i = this->allMoves.size() - 1; i >= 0; i--) {
		this->undoMove();
	}
}
bool Board::replay() {
	if (this->reMoves.size() != 0) {
		this->redoMove();
		return true;
	}

}
void Board::setVolume(float x)
{
	this->sound->setVolume(x);
}
