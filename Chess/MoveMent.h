#pragma once
#include"ChessPiece.h"
#include"ChessCell.h"
class MoveMent {
private:
	ChessCell c1;
	int c1_;
	ChessCell c2;
	int c2_;
	ChessCell tempMove;
	int cM_;
	ChessCell tempKill;
	int cK_;
public:
	MoveMent();
	~MoveMent();
	void setC1(ChessCell);
	void setC2(ChessCell);
	void setTempKill(ChessCell);
	void setTempMove(ChessCell);
	ChessCell getC2();
	ChessCell getC1();
	ChessCell getTempKill();
	ChessCell getTempMove();

	void setC1_(int);
	void setC2_(int);
	void setTempKill_(int);
	void setTempMove_(int);
	int getC2_();
	int getC1_();
	int getTempKill_();
	int getTempMove_();

};