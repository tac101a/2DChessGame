#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "ChessCell.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include"MoveMent.h"
#include"Sound.h"
#include "Connector.h"
using namespace std;


class Board :public sf::Drawable
{
public:
	//-------CONSTRUCTOR & GRAPHIC SETTING---------
	//constructor:
	Board(sf::Color c1 = sf::Color{ 250,245,246 }, sf::Color c2 = sf::Color{ 255,142,176 });
	//chesscell's color setup
	void display(sf::Color c1 = sf::Color{ 250,245,246 }, sf::Color c2 = sf::Color{ 255,142,176 });
	//destructor:
	~Board();
	//set Piece on Board 
	void setPiece(int x, int y, ChessPiece* cp);

	//set color massively
	void massiveSetColor(vector<int> vct, sf::Color c1);

	//------GAME HANDLING--------
	//handling move
	void moveHandle(sf::Vector2f v);
	//get Coordinates of all pieces in board
	void mapCalculate();
	//checking if a king is attacked:
	bool isChecked(ChessPiece* cp);

	//convert numbers to a b c 
	string log(int i);
	//convert a b c... to numbers
	int unlog(string position);
	// save the moves to file
	void saveChess(stringstream &s);
	// handling move with computer
	void moveHandleForComputer(sf::Vector2f v);

	
	void redoMove();
	void undoMove();

	
	void print();
	// if new move is made, delete move save in vector allMoves and reMoves
	void resetMove();
	
	bool replay();
	// undo until board return to original
	void resetPlay();

	static int checkMate_;

	// reset file save chess.
	void resetSaveChess();
	//set volume sound 
	void setVolume(float x);
private:
	// sound move, kill, check, checkmate
	Sound* sound;

	ChessCell* square; //chessBoard
	int cellPressed; //value of the pressed cell/ -1 if no cell pressed
	
	vector<int> blackCoords; //coordinates of all black pieces
	vector<int> whiteCoords; //coordinates of all white pieces

	int kingCoords[2]; //coordinates of black and white king
	

	bool isBlackTurn; // True if it is black's turn

	//------SUPPORT MOVE HANDLING-------
	//if there were more than 1 cell chosen
	bool isMassiveChose();
	//if chosen cell is included in possibleMove
	bool moveIncluded(int i);
	//completing the move
	void pieceShifting(int i);
	void pieceShifting_un(int i,int j);
	void pieceShifting_re(int i, int j);
	//--------KING'S POSSIBLE MOVE----------

	//intersect of 2 vectors move
	vector<int> intersect(vector<int> v1, vector<int> v2);

	//get all moves that will put a king in a check:
	void possibleCheck();
	
	//----STATE CALCULATE------
	//int stateCal(sf::Vector2f v);

	//----INHERIT METHOD-------
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// behind move to check 
	void checkKing(ChessPiece* piece);
	// if chess is castle ?
	void possibleCastle(ChessPiece* cp, vector<int> bCoords, vector<int> wCoords);

	
	bool Checked_(ChessPiece* king);

	bool isChecked_(ChessPiece* cp,int pos);

	void checkMate();
	
	// pawn cross kill 
	void crossKillSpecial(ChessPiece* cp, vector<int> bCoords, vector<int> wCoords);
	// promote pawn
	void Promote_(ChessPiece* cp, MoveMent& move);
	

	vector<MoveMent> allMoves; // Vector lưu tất cả nước đi.
	vector<MoveMent> reMoves;
	int moveAt =0;
	int sizeMove = 0;
	int numMove = 0;
};


